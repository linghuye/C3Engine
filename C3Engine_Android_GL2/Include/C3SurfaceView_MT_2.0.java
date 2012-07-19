/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.Tq.C3Engine;

import java.io.Writer;
import java.util.ArrayList;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLDebugHelper;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class C3SurfaceView extends SurfaceView implements SurfaceHolder.Callback 
{
    private final static boolean LOG_THREADS = false;
    private final static boolean LOG_SURFACE = false;
    private final static boolean LOG_EGL = false;
    // Work-around for bug 2263168
    private final static boolean DRAW_TWICE_AFTER_SIZE_CHANGED = true;
    /**
     * when the surface is created, or when {@link #requestRender} is called.
     *
     * @see #getRenderMode()
     * @see #setRenderMode(int)
     * @see #requestRender()
     */
    public final static int RENDERMODE_WHEN_DIRTY = 0;
    /**
     * The renderer is called
     * continuously to re-render the scene.
     *
     * @see #getRenderMode()
     * @see #setRenderMode(int)
     */
    public final static int RENDERMODE_CONTINUOUSLY = 1;

    /**
     * Standard View constructor. In order to render something, you
     * must call {@link #setRenderer} to register a renderer.
     */
    public C3SurfaceView(Context context) 
    {
        super(context);
        init();
    }

    /**
     * Standard View constructor. In order to render something, you
     * must call {@link #setRenderer} to register a renderer.
     */
    public C3SurfaceView(Context context, AttributeSet attrs) 
    {
        super(context, attrs);
        init();
    }

    private void init() 
    {
        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed
        SurfaceHolder holder = getHolder();
        holder.addCallback(this);
        
        // 设置默认像素格式,后续可以更改.
        holder.setFormat(PixelFormat.RGB_565);
        
        // setType is not needed for SDK 2.0 or newer. Uncomment this
        // statement if back-porting this code to older SDKs.
        holder.setType(SurfaceHolder.SURFACE_TYPE_GPU);
    }

    /**
     * Set the glWrapper. If the glWrapper is not null, its
     * {@link GLWrapper#wrap(GL)} method is called
     * whenever a surface is created. A GLWrapper can be used to wrap
     * the GL object that's passed to the renderer. Wrapping a GL
     * object enables examining and modifying the behavior of the
     * GL calls made by the renderer.
     * <p>
     * Wrapping is typically used for debugging purposes.
     * <p>
     * The default value is null.
     * @param glWrapper the new GLWrapper
     */
    public void setGLWrapper(GLWrapper glWrapper) 
    {
        mGLWrapper = glWrapper;
    }

    /**
     * Set the renderer associated with this view. Also starts the thread that
     * will call the renderer, which in turn causes the rendering to start.
     * <p>This method should be called once and only once in the life-cycle of
     * a GLSurfaceView.
     * <p>The following GLSurfaceView methods can only be called <em>before</em>
     * setRenderer is called:
     * <ul>
     * <li>{@link #setEGLConfigChooser(boolean)}
     * <li>{@link #setEGLConfigChooser(EGLConfigChooser)}
     * <li>{@link #setEGLConfigChooser(int, int, int, int, int, int)}
     * </ul>
     * <p>
     * The following GLSurfaceView methods can only be called <em>after</em>
     * setRenderer is called:
     * <ul>
     * <li>{@link #getRenderMode()}
     * <li>{@link #onPause()}
     * <li>{@link #onResume()}
     * <li>{@link #queueEvent(Runnable)}
     * <li>{@link #requestRender()}
     * <li>{@link #setRenderMode(int)}
     * </ul>
     *
     * @param renderer the renderer to use to perform OpenGL drawing.
     */
    public void setRenderer(Renderer renderer)
    {
        checkRenderThreadState();
        if (mEGLConfigChooser == null) 
        {
            mEGLConfigChooser = new SimpleEGLConfigChooser(true);
        }
        if (mEGLContextFactory == null) 
        {
            mEGLContextFactory = new DefaultContextFactory();
        }
        if (mEGLWindowSurfaceFactory == null) 
        {
            mEGLWindowSurfaceFactory = new DefaultWindowSurfaceFactory();
        }
        
        // !!!在这里启动线程
        mGLThread = new GLThread(renderer);
        mGLThread.start();
    }

    /**
     * Install a custom EGLContextFactory.
     * <p>If this method is
     * called, it must be called before {@link #setRenderer(Renderer)}
     * is called.
     * <p>
     * If this method is not called, then by default
     * a context will be created with no shared context and
     * with a null attribute list.
     */
    public void setEGLContextFactory(EGLContextFactory factory) 
    {
        checkRenderThreadState();
        mEGLContextFactory = factory;
    }

    /**
     * Install a custom EGLWindowSurfaceFactory.
     * <p>If this method is
     * called, it must be called before {@link #setRenderer(Renderer)}
     * is called.
     * <p>
     * If this method is not called, then by default
     * a window surface will be created with a null attribute list.
     */
    public void setEGLWindowSurfaceFactory(EGLWindowSurfaceFactory factory) 
    {
        checkRenderThreadState();
        mEGLWindowSurfaceFactory = factory;
    }

    /**
     * Install a custom EGLConfigChooser.
     * <p>If this method is
     * called, it must be called before {@link #setRenderer(Renderer)}
     * is called.
     * <p>
     * If no setEGLConfigChooser method is called, then by default the
     * view will choose an EGLConfig that is compatible with the current
     * android.view.Surface, with a depth buffer depth of
     * at least 16 bits.
     * @param configChooser
     */
    public void setEGLConfigChooser(EGLConfigChooser configChooser) 
    {
        checkRenderThreadState();
        mEGLConfigChooser = configChooser;
    }

    /**
     * Install a config chooser which will choose a config
     * as close to 16-bit RGB as possible, with or without an optional depth
     * buffer as close to 16-bits as possible.
     * <p>If this method is
     * called, it must be called before {@link #setRenderer(Renderer)}
     * is called.
     * <p>
     * If no setEGLConfigChooser method is called, then by default the
     * view will choose an RGB_565 surface with a depth buffer depth of
     * at least 16 bits.
     *
     * @param needDepth
     */
    public void setEGLConfigChooser(boolean needDepth)
    {
        setEGLConfigChooser(new SimpleEGLConfigChooser(needDepth));
    }

    /**
     * Install a config chooser which will choose a config
     * with at least the specified depthSize and stencilSize,
     * and exactly the specified redSize, greenSize, blueSize and alphaSize.
     * <p>If this method is
     * called, it must be called before {@link #setRenderer(Renderer)}
     * is called.
     * <p>
     * If no setEGLConfigChooser method is called, then by default the
     * view will choose an RGB_565 surface with a depth buffer depth of
     * at least 16 bits.
     *
     */
    public void setEGLConfigChooser(int redSize, int greenSize, int blueSize,
            int alphaSize, int depthSize, int stencilSize) 
    {
        setEGLConfigChooser(new ComponentSizeChooser(redSize, greenSize,
                blueSize, alphaSize, depthSize, stencilSize));
    }

    /**
     * Inform the default EGLContextFactory and default EGLConfigChooser
     * which EGLContext client version to pick.
     * <p>Use this method to create an OpenGL ES 2.0-compatible context.
     * Example:
     * <pre class="prettyprint">
     *     public MyView(Context context) {
     *         super(context);
     *         setEGLContextClientVersion(2); // Pick an OpenGL ES 2.0 context.
     *         setRenderer(new MyRenderer());
     *     }
     * </pre>
     * <p>Note: Activities which require OpenGL ES 2.0 should indicate this by
     * setting @lt;uses-feature android:glEsVersion="0x00020000" /> in the activity's
     * AndroidManifest.xml file.
     * <p>If this method is called, it must be called before {@link #setRenderer(Renderer)}
     * is called.
     * <p>This method only affects the behavior of the default EGLContexFactory and the
     * default EGLConfigChooser. If
     * {@link #setEGLContextFactory(EGLContextFactory)} has been called, then the supplied
     * EGLContextFactory is responsible for creating an OpenGL ES 2.0-compatible context.
     * If
     * {@link #setEGLConfigChooser(EGLConfigChooser)} has been called, then the supplied
     * EGLConfigChooser is responsible for choosing an OpenGL ES 2.0-compatible config.
     * @param version The EGLContext client version to choose. Use 2 for OpenGL ES 2.0
     */
    public void setEGLContextClientVersion(int version) 
    {
        checkRenderThreadState();
        mEGLContextClientVersion = version;
    }

    /**
     * Set the rendering mode. When renderMode is
     * RENDERMODE_CONTINUOUSLY, the renderer is called
     * repeatedly to re-render the scene. When renderMode
     * is RENDERMODE_WHEN_DIRTY, the renderer only rendered when the surface
     * is created, or when {@link #requestRender} is called. Defaults to RENDERMODE_CONTINUOUSLY.
     * <p>
     * Using RENDERMODE_WHEN_DIRTY can improve battery life and overall system performance
     * by allowing the GPU and CPU to idle when the view does not need to be updated.
     * <p>
     * This method can only be called after {@link #setRenderer(Renderer)}
     *
     * @param renderMode one of the RENDERMODE_X constants
     * @see #RENDERMODE_CONTINUOUSLY
     * @see #RENDERMODE_WHEN_DIRTY
     */
    public void setRenderMode(int renderMode) 
    {
        mGLThread.setRenderMode(renderMode);
    }

    /**
     * Get the current rendering mode. May be called
     * from any thread. Must not be called before a renderer has been set.
     * @return the current rendering mode.
     * @see #RENDERMODE_CONTINUOUSLY
     * @see #RENDERMODE_WHEN_DIRTY
     */
    public int getRenderMode() 
    {
        return mGLThread.getRenderMode();
    }

    /**
     * Request that the renderer render a frame.
     * This method is typically used when the render mode has been set to
     * {@link #RENDERMODE_WHEN_DIRTY}, so that frames are only rendered on demand.
     * May be called
     * from any thread. Must not be called before a renderer has been set.
     */
    public void requestRender() 
    {
        mGLThread.requestRender();
    }

    /**
     * This method is part of the SurfaceHolder.Callback interface, and is
     * not normally called or subclassed by clients of GLSurfaceView.
     */
    public void surfaceCreated(SurfaceHolder holder)
    {
    	// 异步通知线程
    	// 每次resume时Surface都被重新创建
        mGLThread.surfaceCreated();
    }

    /**
     * This method is part of the SurfaceHolder.Callback interface, and is
     * not normally called or subclassed by clients of GLSurfaceView.
     */
    public void surfaceDestroyed(SurfaceHolder holder)
    {	
        // Surface will be destroyed when we return
    	// 每次pause时Surface都被销毁
        mGLThread.surfaceDestroyed();
    }

    /**
     * This method is part of the SurfaceHolder.Callback interface, and is
     * not normally called or subclassed by clients of GLSurfaceView.
     */
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) 
    {
        mGLThread.onWindowResize(w, h);
    }

    /**
     * Inform the view that the activity is paused. The owner of this view must
     * call this method when the activity is paused. Calling this method will
     * pause the rendering thread.
     * Must not be called before a renderer has been set.
     */
    public void onPause() 
    {
        mGLThread.onPause();
    }

    /**
     * Inform the view that the activity is resumed. The owner of this view must
     * call this method when the activity is resumed. Calling this method will
     * recreate the OpenGL display and resume the rendering thread.
     * Must not be called before a renderer has been set.
     */
    public void onResume() 
    {
        mGLThread.onResume();
    }

    /**
     * Queue a runnable to be run on the GL rendering thread. This can be used
     * to communicate with the Renderer on the rendering thread.
     * Must not be called before a renderer has been set.
     * @param r the runnable to be run on the GL rendering thread.
     */
    public void queueEvent(Runnable r)
    {
        mGLThread.queueEvent(r);
    }

    /**
     * This method is used as part of the View class and is not normally
     * called or subclassed by clients of GLSurfaceView.
     * Must not be called before a renderer has been set.
     */
    @Override
    protected void onDetachedFromWindow() 
    {
        super.onDetachedFromWindow();
        mGLThread.requestExitAndWait();
    }

    // ----------------------------------------------------------------------

    /**
     * An interface used to wrap a GL interface.
     * <p>Typically
     * used for implementing debugging and tracing on top of the default
     * GL interface. You would typically use this by creating your own class
     * that implemented all the GL methods by delegating to another GL instance.
     * Then you could add your own behavior before or after calling the
     * delegate. All the GLWrapper would do was instantiate and return the
     * wrapper GL instance:
     * <pre class="prettyprint">
     * class MyGLWrapper implements GLWrapper {
     *     GL wrap(GL gl) {
     *         return new MyGLImplementation(gl);
     *     }
     *     static class MyGLImplementation implements GL,GL10,GL11,... {
     *         ...
     *     }
     * }
     * </pre>
     * @see #setGLWrapper(GLWrapper)
     */
    public interface GLWrapper 
    {
        /**
         * Wraps a gl interface in another gl interface.
         * @param gl a GL interface that is to be wrapped.
         * @return either the input argument or another GL object that wraps the input argument.
         */
        GL wrap(GL gl);
    }

    /**
     * A generic renderer interface.
     * <p>
     * The renderer is responsible for making OpenGL calls to render a frame.
     * <p>
     * GLSurfaceView clients typically create their own classes that implement
     * this interface, and then call {@link C3SurfaceView#setRenderer} to
     * register the renderer with the GLSurfaceView.
     * <p>
     * <h3>Threading</h3>
     * The renderer will be called on a separate thread, so that rendering
     * performance is decoupled from the UI thread. Clients typically need to
     * communicate with the renderer from the UI thread, because that's where
     * input events are received. Clients can communicate using any of the
     * standard Java techniques for cross-thread communication, or they can
     * use the {@link C3SurfaceView#queueEvent(Runnable)} convenience method.
     * <p>
     * <h3>EGL Context Lost</h3>
     * There are situations where the EGL rendering context will be lost. This
     * typically happens when device wakes up after going to sleep. When
     * the EGL context is lost, all OpenGL resources (such as textures) that are
     * associated with that context will be automatically deleted. In order to
     * keep rendering correctly, a renderer must recreate any lost resources
     * that it still needs. The {@link #onSurfaceCreated(GL10, EGLConfig)} method
     * is a convenient place to do this.
     *
     *
     * @see #setRenderer(Renderer)
     */
    // Note: 此接口由C3底层设计
    public interface Renderer
    {
        void onSurfaceCreated(int width, int height);
        void onSurfaceChanged(int width, int height);
        void onDrawFrame();
        void onFinish();
    }

    /**
     * An interface for customizing the eglCreateContext and eglDestroyContext calls.
     * <p>
     * This interface must be implemented by clients wishing to call
     * {@link C3SurfaceView#setEGLContextFactory(EGLContextFactory)}
     */
    public interface EGLContextFactory 
    {
        EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig);
        void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context);
    }

    // 默认Context工厂
    private class DefaultContextFactory implements EGLContextFactory
    {
        private int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig config)
        {
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, mEGLContextClientVersion,
                    EGL10.EGL_NONE };

            return egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT,
                    mEGLContextClientVersion != 0 ? attrib_list : null);
        }

        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) 
        {
            if (!egl.eglDestroyContext(display, context)) 
            {
                Log.e("DefaultContextFactory", "display:" + display + " context: " + context);
                if (LOG_THREADS) 
                {
                    Log.i("DefaultContextFactory", "tid=" + Thread.currentThread().getId());
                }
                throw new RuntimeException("eglDestroyContext failed: "/* + EGLLogWrapper.getErrorString(egl.eglGetError())*/);
            }
        }
    }

    /**
     * An interface for customizing the eglCreateWindowSurface and eglDestroySurface calls.
     * <p>
     * This interface must be implemented by clients wishing to call
     * {@link C3SurfaceView#setEGLWindowSurfaceFactory(EGLWindowSurfaceFactory)}
     */
    public interface EGLWindowSurfaceFactory 
    {
        EGLSurface createWindowSurface(EGL10 egl, EGLDisplay display, EGLConfig config,
                Object nativeWindow);
        void destroySurface(EGL10 egl, EGLDisplay display, EGLSurface surface);
    }

    private static class DefaultWindowSurfaceFactory implements EGLWindowSurfaceFactory 
    {
        public EGLSurface createWindowSurface(EGL10 egl, EGLDisplay display, EGLConfig config, Object nativeWindow) 
        {
            return egl.eglCreateWindowSurface(display, config, nativeWindow, null);
        }

        public void destroySurface(EGL10 egl, EGLDisplay display, EGLSurface surface) 
        {
            egl.eglDestroySurface(display, surface);
        }
    }

    /**
     * An interface for choosing an EGLConfig configuration from a list of
     * potential configurations.
     * <p>
     * This interface must be implemented by clients wishing to call
     * {@link C3SurfaceView#setEGLConfigChooser(EGLConfigChooser)}
     */
    public interface EGLConfigChooser 
    {
        /**
         * Choose a configuration from the list. Implementors typically
         * implement this method by calling
         * {@link EGL10#eglChooseConfig} and iterating through the results. Please consult the
         * EGL specification available from The Khronos Group to learn how to call eglChooseConfig.
         * @param egl the EGL10 for the current display.
         * @param display the current display.
         * @return the chosen configuration.
         */
        EGLConfig chooseConfig(EGL10 egl, EGLDisplay display);
    }

    private abstract class BaseConfigChooser implements EGLConfigChooser 
    {
        public BaseConfigChooser(int[] configSpec) 
        {
            mConfigSpec = filterConfigSpec(configSpec);
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) 
        {
            int[] num_config = new int[1];
            if (!egl.eglChooseConfig(display, mConfigSpec, null, 0, num_config)) 
            {
                throw new IllegalArgumentException("eglChooseConfig failed");
            }

            int numConfigs = num_config[0];

            if (numConfigs <= 0) 
            {
                throw new IllegalArgumentException("No configs match configSpec");
            }

            EGLConfig[] configs = new EGLConfig[numConfigs];
            if (!egl.eglChooseConfig(display, mConfigSpec, configs, numConfigs, num_config)) 
            {
                throw new IllegalArgumentException("eglChooseConfig#2 failed");
            }
            EGLConfig config = chooseConfig(egl, display, configs);
            if (config == null) 
            {
                throw new IllegalArgumentException("No config chosen");
            }
            return config;
        }

        abstract EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs);

        protected int[] mConfigSpec;

        private int[] filterConfigSpec(int[] configSpec) 
        {
            if (mEGLContextClientVersion != 2) 
            {
                return configSpec;
            }
            /* We know none of the subclasses define EGL_RENDERABLE_TYPE.
             * And we know the configSpec is well formed.
             */
            int len = configSpec.length;
            int[] newConfigSpec = new int[len + 2];
            System.arraycopy(configSpec, 0, newConfigSpec, 0, len-1);
            newConfigSpec[len-1] = EGL10.EGL_RENDERABLE_TYPE;
            newConfigSpec[len] = 4; /* EGL_OPENGL_ES2_BIT */
            newConfigSpec[len+1] = EGL10.EGL_NONE;
            return newConfigSpec;
        }
    }

    /**
     * Choose a configuration with exactly the specified r,g,b,a sizes,
     * and at least the specified depth and stencil sizes.
     */
    private class ComponentSizeChooser extends BaseConfigChooser 
    {
        public ComponentSizeChooser(int redSize, int greenSize, int blueSize, int alphaSize, int depthSize, int stencilSize) 
        {
            super(new int[] 
            {
                    EGL10.EGL_RED_SIZE, redSize,
                    EGL10.EGL_GREEN_SIZE, greenSize,
                    EGL10.EGL_BLUE_SIZE, blueSize,
                    EGL10.EGL_ALPHA_SIZE, alphaSize,
                    EGL10.EGL_DEPTH_SIZE, depthSize,
                    EGL10.EGL_STENCIL_SIZE, stencilSize,
                    // 打开MultiSample抗锯齿,对高端机器使用.
              //    EGL10.EGL_SAMPLE_BUFFERS, 1,
              //    EGL10.EGL_SAMPLES, 4,
                    EGL10.EGL_NONE
            });
            mConfigValue = new int[1];
            mRedSize = redSize;
            mGreenSize = greenSize;
            mBlueSize = blueSize;
            mAlphaSize = alphaSize;
            mDepthSize = depthSize;
            mStencilSize = stencilSize;
       }

        @Override
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs) 
        {
        	// 要求RGBA一致,但d/s只要不小于.
            for (EGLConfig config : configs) 
            {
                int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);
                if ((d >= mDepthSize) && (s >= mStencilSize)) 
                {
                    int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);
                    int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
                    int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
                    int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);
                    if ((r >= mRedSize) && (g >= mGreenSize) && (b >= mBlueSize) && (a == mAlphaSize)) 
                    {
                        return config;
                    }
                }
            }
            return null;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                EGLConfig config, int attribute, int defaultValue) 
        {

            if (egl.eglGetConfigAttrib(display, config, attribute, mConfigValue)) 
            {
                return mConfigValue[0];
            }
            return defaultValue;
        }

        private int[] mConfigValue;
        // Subclasses can adjust these values:
        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
    }

    /**
     * This class will choose a RGB_565 surface with
     * or without a depth buffer.
     *
     */
    private class SimpleEGLConfigChooser extends ComponentSizeChooser 
    {
        public SimpleEGLConfigChooser(boolean withDepthBuffer) 
        {
            super(5, 6, 5, 0, withDepthBuffer ? 16 : 0, 0);
        }
    }
    
/*	public class MultisampleConfigChooser implements EGLConfigChooser 
	{
	   static private final String kTag = "GDC11";
	   
	   public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) 
	   {
	       mConfigValue = new int[1];
	
	       // Try to find a normal multisample configuration first.
	       int[] configSpec = 
	       {
    		   EGL10.EGL_RED_SIZE, 5,
               EGL10.EGL_GREEN_SIZE, 6,
               EGL10.EGL_BLUE_SIZE, 5,
               EGL10.EGL_DEPTH_SIZE, 16,
               // Requires that setEGLContextClientVersion(2) is called on the view.
               EGL10.EGL_RENDERABLE_TYPE, 4,
               EGL10.EGL_SAMPLE_BUFFERS, 1,
               EGL10.EGL_SAMPLES, 2,
               EGL10.EGL_NONE
	       };
	
	       if(!egl.eglChooseConfig(display, configSpec, null, 0, mConfigValue)) 
	       {
	           throw new IllegalArgumentException("eglChooseConfig failed");
	       }
	       
	       int numConfigs = mConfigValue[0];
	       if(numConfigs <= 0) 
	       {
	           // No normal multisampling config was found. Try to create a
	           // converage multisampling configuration, for the nVidia Tegra2.
	           final int EGL_COVERAGE_BUFFERS_NV = 0x30E0;
	           final int EGL_COVERAGE_SAMPLES_NV = 0x30E1;
	
	           configSpec = new int[]
	           {
	              EGL10.EGL_RED_SIZE, 5,
	              EGL10.EGL_GREEN_SIZE, 6,
	              EGL10.EGL_BLUE_SIZE, 5,
	              EGL10.EGL_DEPTH_SIZE, 16,
	              EGL10.EGL_RENDERABLE_TYPE, 4,
	              EGL_COVERAGE_BUFFERS_NV, 1,
	              EGL_COVERAGE_SAMPLES_NV, 2, 
	              EGL10.EGL_NONE
	           };
	
	           if (!egl.eglChooseConfig(display, configSpec, null, 0, mConfigValue)) 
	           {
	               throw new IllegalArgumentException("2nd eglChooseConfig failed");
	           }
	           numConfigs = mConfigValue[0];
	
	           if (numConfigs <= 0) 
	           {
	               // Give up, try without multisampling.
	               configSpec = new int[]
	               {
	                   EGL10.EGL_RED_SIZE, 5,
	                   EGL10.EGL_GREEN_SIZE, 6,
	                   EGL10.EGL_BLUE_SIZE, 5,
	                   EGL10.EGL_DEPTH_SIZE, 16,
	                   EGL10.EGL_RENDERABLE_TYPE, 4,
	                   EGL10.EGL_NONE
	               };
	
	               if(!egl.eglChooseConfig(display, configSpec, null, 0, mConfigValue)) 
	               {
	                   throw new IllegalArgumentException("3rd eglChooseConfig failed");
	               }
	               numConfigs = mConfigValue[0];
	
	               if (numConfigs <= 0) 
	               {
	                 throw new IllegalArgumentException("No configs match configSpec");
	               }
	           }
	       }
	
	       // Get all matching configurations.
	       EGLConfig[] configs = new EGLConfig[numConfigs];
	       if (!egl.eglChooseConfig(display, configSpec, configs, numConfigs, mConfigValue)) 
	       {
	           throw new IllegalArgumentException("data eglChooseConfig failed");
	       }
	
	       // CAUTION! eglChooseConfigs returns configs with higher bit depth
	       // first: Even though we asked for rgb565 configurations, rgb888
	       // configurations are considered to be "better" and returned first.
	       // You need to explicitly filter the data returned by eglChooseConfig!
	       int index = -1;
	       for (int i = 0; i < configs.length; ++i) 
	       {
	           if (findConfigAttrib(egl, display, configs[i], EGL10.EGL_RED_SIZE, 0) == 5) 
	           {
	               index = i;
	               break;
	           }
	       }
	       if (index == -1) 
	       {
	           Log.w(kTag, "Did not find sane config, using first");
	       }
	       EGLConfig config = configs.length > 0 ? configs[index] : null;
	       if (config == null) 
	       {
	           throw new IllegalArgumentException("No config chosen");
	       }
	       return config;
	   }
	
	   private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue) 
	   {
	       if (egl.eglGetConfigAttrib(display, config, attribute, mConfigValue)) 
	       {
	           return mConfigValue[0];
	       }
	       return defaultValue;
	   }
	   private int[] mConfigValue;
	}
*/
    /**
     * An EGL helper class.
     */

    private class EglHelper
    {
        public EglHelper() 
        {
        }

        /**
         * Initialize EGL for a given configuration spec.
         * @param configSpec
         */
        public void CreateEglContext() 
        {
            if (LOG_EGL) 
            {
                Log.w("EglHelper", "start() tid=" + Thread.currentThread().getId());
            }
            /*
             * Get an EGL instance
             */
            mEgl = (EGL10) EGLContext.getEGL();

            /*
             * Get to the default display.
             */
            mEglDisplay = mEgl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);

            if (mEglDisplay == EGL10.EGL_NO_DISPLAY) 
            {
                throw new RuntimeException("eglGetDisplay failed");
            }

            /*
             * We can now initialize EGL for that display
             */
            int[] version = new int[2];
            if(!mEgl.eglInitialize(mEglDisplay, version))
            {
                throw new RuntimeException("eglInitialize failed");
            }
            mEglConfig = mEGLConfigChooser.chooseConfig(mEgl, mEglDisplay);

            /*
            * Create an EGL context. We want to do this as rarely as we can, because an
            * EGL context is a somewhat heavy object.
            */
            mEglContext = mEGLContextFactory.createContext(mEgl, mEglDisplay, mEglConfig);
            if (mEglContext == null || mEglContext == EGL10.EGL_NO_CONTEXT) 
            {
                mEglContext = null;
                throwEglException("createContext");
            }
            if (LOG_EGL) 
            {
                Log.w("EglHelper", "createContext " + mEglContext + " tid=" + Thread.currentThread().getId());
            }

            mEglSurface = null;
        }

        /*
         * React to the creation of a new surface by creating and returning an
         * OpenGL interface that renders to that surface.
         */
        public GL createSurface(SurfaceHolder holder) 
        {
            if (LOG_EGL) 
            {
                Log.w("EglHelper", "createSurface()  tid=" + Thread.currentThread().getId());
            }
            /*
             * Check preconditions.
             */
            if (mEgl == null) 
            {
                throw new RuntimeException("egl not initialized");
            }
            if (mEglDisplay == null) 
            {
                throw new RuntimeException("eglDisplay not initialized");
            }
            if (mEglConfig == null)
            {
                throw new RuntimeException("mEglConfig not initialized");
            }
            /*
             *  The window size has changed, so we need to create a new
             *  surface.
             */
            if (mEglSurface != null && mEglSurface != EGL10.EGL_NO_SURFACE) 
            {

                /*
                 * Unbind and destroy the old EGL surface, if
                 * there is one.
                 */
                mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
                        EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
                mEGLWindowSurfaceFactory.destroySurface(mEgl, mEglDisplay, mEglSurface);
            }

            /*
             * Create an EGL surface we can render into.
             */
            mEglSurface = mEGLWindowSurfaceFactory.createWindowSurface(mEgl, mEglDisplay, mEglConfig, holder);

            if (mEglSurface == null || mEglSurface == EGL10.EGL_NO_SURFACE) 
            {
                int error = mEgl.eglGetError();
                if (error == EGL10.EGL_BAD_NATIVE_WINDOW) 
                {
                    Log.e("EglHelper", "createWindowSurface returned EGL_BAD_NATIVE_WINDOW.");
                    return null;
                }
                throwEglException("createWindowSurface", error);
            }

            /*
             * Before we can issue GL commands, we need to make sure
             * the context is current and bound to a surface.
             */
            if (!mEgl.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext))
            {
                throwEglException("eglMakeCurrent");
            }

            GL gl = mEglContext.getGL();
            if (mGLWrapper != null) 
            {
                gl = mGLWrapper.wrap(gl);
            }

            return gl;
        }

        /**
         * Display the current render surface.
         * @return false if the context has been lost.
         */
        public boolean swap() 
        {
            if (! mEgl.eglSwapBuffers(mEglDisplay, mEglSurface)) 
            {
                /*
                 * Check for EGL_CONTEXT_LOST, which means the context
                 * and all associated data were lost (For instance because
                 * the device went to sleep). We need to sleep until we
                 * get a new surface.
                 */
                int error = mEgl.eglGetError();
                switch(error) 
                {
                case EGL11.EGL_CONTEXT_LOST:
                    return false;
                case EGL10.EGL_BAD_NATIVE_WINDOW:
                    // The native window is bad, probably because the
                    // window manager has closed it. Ignore this error,
                    // on the expectation that the application will be closed soon.
                    Log.e("EglHelper", "eglSwapBuffers returned EGL_BAD_NATIVE_WINDOW. tid=" + Thread.currentThread().getId());
                    break;
                default:
                    throwEglException("eglSwapBuffers", error);
                }
            }
            return true;
        }

        public void destroySurface() 
        {
            if(LOG_EGL) 
            {
                Log.w("EglHelper", "destroySurface()  tid=" + Thread.currentThread().getId());
            }
            
            if(mEglSurface != null && mEglSurface != EGL10.EGL_NO_SURFACE)
            {
                mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
                mEGLWindowSurfaceFactory.destroySurface(mEgl, mEglDisplay, mEglSurface);
                mEglSurface = null;
            }
        }

        public void finish() 
        {
            if (LOG_EGL) 
            {
                Log.w("EglHelper", "finish() tid=" + Thread.currentThread().getId());
            }
            
            if(mEglDisplay != null)
            {
            	mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
            }
            
            if (mEglContext != null) 
            {
                mEGLContextFactory.destroyContext(mEgl, mEglDisplay, mEglContext);
                mEglContext = null;
            }
            
            if (mEglDisplay != null) 
            {
                mEgl.eglTerminate(mEglDisplay);
                mEglDisplay = null;
            }
        }

        private void throwEglException(String function) 
        {
            throwEglException(function, mEgl.eglGetError());
        }

        private void throwEglException(String function, int error)
        {
            String message = function + " failed: ";/* + EGLLogWrapper.getErrorString(error)*/
            if(LOG_THREADS) 
            {
                Log.e("EglHelper", "throwEglException tid=" + Thread.currentThread().getId() + " " + message);
            }
            throw new RuntimeException(message);
        }

        EGL10 mEgl;
        EGLDisplay mEglDisplay;
        EGLSurface mEglSurface;
        EGLConfig mEglConfig;
        EGLContext mEglContext;
    }

    /**
     * A generic GL Thread. Takes care of initializing EGL and GL. Delegates
     * to a Renderer instance to do the actual drawing. Can be configured to
     * render continuously or on request.
     *
     * All potentially blocking synchronization is done through the
     * sGLThreadManager object. This avoids multiple-lock ordering issues.
     *
     */
    class GLThread extends Thread 
    {
        GLThread(Renderer renderer) 
        {
            super();
            mWidth = 0;
            mHeight = 0;
            mRequestRender = true;
            mRenderMode = RENDERMODE_CONTINUOUSLY;
            mRenderer = renderer;
        }


        @Override
        public void run() 
        {
            setName("GLThread " + getId());
            if (LOG_THREADS) 
            {
                Log.i("GLThread", "starting tid=" + getId());
            }
            
            try 
            {
                guardedRun();
            } 
            catch (InterruptedException e) 
            {
                // fall thru and exit normally
            } 
            finally 
            {
                sGLThreadManager.threadExiting(this);
            }
        }

        /*
         * This private method should only be called inside a
         * synchronized(sGLThreadManager) block.
         */
        private void stopEglSurfaceLocked() 
        {
            if(mHaveEglSurface) 
            {
                mHaveEglSurface = false;
                mEglHelper.destroySurface();
            }
        }
        
        /*
         * This private method should only be called inside a
         * synchronized(sGLThreadManager) block.
         */
        private void stopEglContextLocked()
        {
            if (mHaveEglContext) 
            {
            	// 通知C3引擎退出
            	mRenderer.onFinish();
            	
            	// 销毁EGLContext和EGL SDK
                mEglHelper.finish();
                
                mHaveEglContext = false;
                sGLThreadManager.releaseEglContextLocked(this);
            }
        }
        
        private void guardedRun() throws InterruptedException
        {
            mEglHelper = new EglHelper();
            mHaveEglContext = false;
            mHaveEglSurface = false;
            try 
            {
                GL10 gl = null;
                boolean justCreatedEglContext = false;	// 刚才创建了EglContext
                boolean askForEglSurface = false;		// 要求EglSurface
                boolean lostEglContext = false;		// EglSurface设备丢失了
                boolean sizeChanged = false;			// surface大小变动了
                boolean wantRenderNotification = false;// 仅用于处理mSizeChanged命令.
                boolean doRenderNotification = false;
                int w = 0;
                int h = 0;
                Runnable event = null;
                
                while(true) 
                {	
                	// 整个while循环分成两个阶段
                	// 这上半段在同步逻辑
                    synchronized(sGLThreadManager) 
                    {
                    	// 同步逻辑直到break.
                        while(true)
                        {
                            if(mShouldExit) 
                            {
                                return;
                            }

                            if(!mEventQueue.isEmpty()) 
                            {
                                event = mEventQueue.remove(0);
                                break;
                            }

                            // Update the pause state.
                            // 在这里接收开始运行和结束运行的状态指示
                            if(mPaused != mRequestPaused) 
                            {
                                mPaused = mRequestPaused;
                                sGLThreadManager.notifyAll();
                            }

                            // Have we lost the EGL context?
                            if (lostEglContext) 
                            {
                                stopEglSurfaceLocked();
                                stopEglContextLocked();
                                lostEglContext = false;
                            }
                            		
                            // 被要求停止运行,进入销毁流程.
                            if (mHaveEglSurface && mPaused) 
                            {
                            	// Note: 此时窗口尚未销毁mHasSurface还在!
                                if(LOG_SURFACE) 
                                {
                                    Log.i("GLThread", "releasing EGL surface because paused tid=" + getId());
                                }
                                
                                // 第1步先销毁EGLSurface
                                // Note: 尽管EGLSurface被删除,但是线程的EGLContext仍然有效!!!
                                // Note: 销毁EGLSurface不需要窗口Surface支撑.
                                stopEglSurfaceLocked();
                                
                                // 第2步销毁EGLContext
                                if(sGLThreadManager.shouldReleaseEGLContextWhenPausing()) 
                                {	
                                	// 销毁EGLContext并解除当前线程绑定的EGLContext
                                	// 同时由于EGLContext被销毁,通知C3引擎退出,因为C3引擎必须有EGLContext支撑.
                                    stopEglContextLocked();
                                    if(LOG_SURFACE) 
                                    {
                                        Log.i("GLThread", "releasing EGL context because paused tid=" + getId());
                                    }
                                }
                            }

                            // Have we lost the surface view surface?
                            // mHasSurface要等到C3SurfaceView收到surfaceCreated通知,再通知GLThread::surfaceCreated后,才得知.
                            if ((!mHasSurface) && (!mWaitingForSurface)) 
                            {
                                if (LOG_SURFACE) 
                                {
                                    Log.i("GLThread", "noticed surfaceView surface lost tid=" + getId());
                                }
                                if (mHaveEglSurface) 
                                {
                                    stopEglSurfaceLocked();
                                }
                                mWaitingForSurface = true;
                                sGLThreadManager.notifyAll();
                            }

                            // Have we acquired the surface view surface?
                            if(mHasSurface && mWaitingForSurface) 
                            {
                                if (LOG_SURFACE) 
                                {
                                    Log.i("GLThread", "noticed surfaceView surface acquired tid=" + getId());
                                }
                                mWaitingForSurface = false;
                                sGLThreadManager.notifyAll();
                            }

                            // 处理mSizeChanged命令
                            if (doRenderNotification) 
                            {
                                wantRenderNotification = false;
                                doRenderNotification = false;
                                mRenderComplete = true;// 主线程得到这个状态后将继续运行
                                sGLThreadManager.notifyAll();
                            }

                            // Ready to draw?
                            if (!mPaused && 
                            	 mHasSurface && (mWidth > 0) && (mHeight > 0) && 
                            	(mRequestRender || (mRenderMode == RENDERMODE_CONTINUOUSLY))) 
                            {	
                                // If we don't have an EGL context, try to acquire one.
                                if(!mHaveEglContext && sGLThreadManager.tryAcquireEglContextLocked(this)) 
                                {	
                                    try
                                    {
                                    	// Note: mEglHelper属于当前线程
                                        mEglHelper.CreateEglContext();
                                    } 
                                    catch(RuntimeException t)
                                    {
                                        sGLThreadManager.releaseEglContextLocked(this);
                                        throw t;
                                    }
                                    mHaveEglContext = true;
                                    justCreatedEglContext = true;

                                    sGLThreadManager.notifyAll();
                                }

                                if (mHaveEglContext && !mHaveEglSurface)
                                {
                                    mHaveEglSurface = true;
                                    askForEglSurface = true;
                                    sizeChanged = true;
                                }

                                if (mHaveEglSurface) 
                                {
                                    if(mSizeChanged) 
                                    {
                                        sizeChanged = true;
                                        w = mWidth;
                                        h = mHeight;
                                        wantRenderNotification = true;

                                        if (DRAW_TWICE_AFTER_SIZE_CHANGED)
                                        {
                                            // We keep mRequestRender true so that we draw twice after the size changes.
                                            // (Once because of mSizeChanged, the second time because of mRequestRender.)
                                            // This forces the updated graphics onto the screen.
                                        } 
                                        else
                                        {
                                            mRequestRender = false;
                                        }
                                        mSizeChanged = false;
                                    } 
                                    else
                                    {
                                        mRequestRender = false;
                                    }
                                    sGLThreadManager.notifyAll();
                                    break;// 在这里break出synchronized(sGLThreadManager)同步逻辑流程,进入到下半段处理. 
                                }
                            }

                            // By design, this is the only place in a GLThread thread where we wait().
                            if (LOG_THREADS) 
                            {
                                Log.i("GLThread", "waiting tid=" + getId());
                            }
                            
                            // 在这里mWaitingForSurface
                            sGLThreadManager.wait();
                        }
                    } // end of synchronized(sGLThreadManager)
                    
                    // 这下半段在处理synchronized得到的执行状态
                    if (event != null) 
                    {
                        event.run();
                        event = null;
                        continue;
                    }
                    
                    // 创建EGL Context在上部分逻辑中完成了.
                    // Note: 流程上是,先有的EglContext和natvie window surface,再有EglSurface.
                    // 但是没有EglSurface之前,无法将EglContext绑定到当前线程!
                    // 没有EglContext无法调用任何OpenGL函数!
                    if (askForEglSurface) 
                    {
                        if (LOG_SURFACE) 
                        {
                            Log.w("GLThread", "egl createSurface");
                        }
                        gl = (GL10) mEglHelper.createSurface(getHolder());
                        if (gl == null) 
                        {
                            // Couldn't create a surface. Quit quietly.
                            break;
                        }
                        sGLThreadManager.checkGLDriver(gl);
                        askForEglSurface = false;
                    }

                    // Note: 之前的mEglHelper.createSurface会设置好当前线程的EglContext!
                    if (justCreatedEglContext)
                    {
                        mRenderer.onSurfaceCreated(w, h);
                        justCreatedEglContext = false;
                    }

                    if (sizeChanged) 
                    {
                        mRenderer.onSurfaceChanged(w, h);
                        sizeChanged = false;
                    }

                    // 渲染当前帧
                    mRenderer.onDrawFrame();
                    
                    // 提交当前帧画面
                    if(!mEglHelper.swap()) 
                    {
                        if(LOG_SURFACE) 
                        {
                            Log.i("GLThread", "egl context lost tid=" + getId());
                        }
                        lostEglContext = true;
                    }

                    if(wantRenderNotification) 
                    {
                        doRenderNotification = true;
                    }
                }
            }
            finally 
            {
                /*
                 * clean-up everything...
                 */
                synchronized (sGLThreadManager) 
                {
                    stopEglSurfaceLocked();
                    stopEglContextLocked();
                }
            }
        }
        
        public void setRenderMode(int renderMode)
        {
            if ( !((RENDERMODE_WHEN_DIRTY <= renderMode) && (renderMode <= RENDERMODE_CONTINUOUSLY)) ) 
            {
                throw new IllegalArgumentException("renderMode");
            }
            synchronized(sGLThreadManager)
            {
                mRenderMode = renderMode;
                sGLThreadManager.notifyAll();
            }
        }
        
        public int getRenderMode()
        {
            synchronized(sGLThreadManager) 
            {
                return mRenderMode;
            }
        }
        
        public void requestRender() 
        {
            synchronized(sGLThreadManager) 
            {
                mRequestRender = true;
                sGLThreadManager.notifyAll();
            }
        }
        
        public void surfaceCreated() 
        {	
            synchronized(sGLThreadManager) 
            {	
                if(LOG_THREADS) 
                {
                    Log.i("GLThread", "surfaceCreated tid=" + getId());
                }
                mHasSurface = true;
                sGLThreadManager.notifyAll();
                while((mWaitingForSurface) && (!mExited)) 
                {
                    try 
                    {
                        sGLThreadManager.wait();
                    }
                    catch (InterruptedException e) 
                    {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }
        
        public void surfaceDestroyed() 
        {
            synchronized(sGLThreadManager) 
            {	
                if(LOG_THREADS) 
                {
                    Log.i("GLThread", "surfaceDestroyed tid=" + getId());
                }
                mHasSurface = false;
                sGLThreadManager.notifyAll();
                while((!mWaitingForSurface) && (!mExited)) 
                {
                    try 
                    {
                        sGLThreadManager.wait();
                    } 
                    catch (InterruptedException e) 
                    {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }
        
        public void onPause() 
        {
        	// mRequestPaused = true导致销毁流程从这里启动!
        	// 实际处理在GLThread中
            synchronized (sGLThreadManager) 
            {
                mRequestPaused = true;
                sGLThreadManager.notifyAll();
                while ((! mExited) && (! mPaused)) 
                {
                    try 
                    {
                        sGLThreadManager.wait();
                    } 
                    catch (InterruptedException ex) 
                    {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }
        
        public void onResume() 
        {
            synchronized (sGLThreadManager) 
            {
                mRequestPaused = false;
                mRequestRender = true;
                mRenderComplete = false;
                sGLThreadManager.notifyAll();
                while ((! mExited) && mPaused && (!mRenderComplete)) 
                {
                    try 
                    {
                        sGLThreadManager.wait();
                    }
                    catch (InterruptedException ex) 
                    {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }
        
        public void onWindowResize(int w, int h) 
        {
            synchronized (sGLThreadManager) 
            {
                mWidth = w;
                mHeight = h;
                mSizeChanged = true;	// 向GLThread发出状态命令,要求处理mSizeChanged和mRequestRender!
                mRequestRender = true;
                mRenderComplete = false;
                sGLThreadManager.notifyAll();

                // Wait for thread to react to resize and render a frame
                while(!mExited && !mPaused && !mRenderComplete) 
                {
                    if (LOG_SURFACE) 
                    {
                        Log.i("Main thread", "onWindowResize waiting for render complete.");
                    }
                    
                    try 
                    {
                        sGLThreadManager.wait();
                    }
                    catch (InterruptedException ex) 
                    {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }
        
        public void requestExitAndWait() 
        {
            // don't call this from GLThread thread or it is a guaranteed
            // deadlock!
            synchronized(sGLThreadManager) 
            {
                mShouldExit = true;
                sGLThreadManager.notifyAll();
                while(!mExited) 
                {
                    try 
                    {
                        sGLThreadManager.wait();
                    }
                    catch (InterruptedException ex) 
                    {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }

        /**
         * Queue an "event" to be run on the GL rendering thread.
         * @param r the runnable to be run on the GL rendering thread.
         */
        public void queueEvent(Runnable r)
        {
            if(r == null) 
            {
                throw new IllegalArgumentException("r must not be null");
            }
            synchronized(sGLThreadManager) 
            {
                mEventQueue.add(r);
                sGLThreadManager.notifyAll();
            }
        }
        
        // Once the thread is started, all accesses to the following member
        // variables are protected by the sGLThreadManager monitor
        private boolean mShouldExit;		// 是否要求退出
        private boolean mExited;			// 是否已退出
        private boolean mRequestPaused;	// 是否要求暂停
        private boolean mPaused;			// 是否正在运行
        private boolean mHasSurface;		// 注意和mHaveEglSurface的区别
        private boolean mWaitingForSurface;// 正在等待C3SurfaceView的surfaceCreated创建通知
        private boolean mHaveEglContext;	// 是否有EglContext
        private boolean mHaveEglSurface;	// 注意和mHasSurface的区别
        private int mWidth;
        private int mHeight;
        private int mRenderMode;
        private boolean mRequestRender;	// 是否要求强制渲染
        private boolean mRenderComplete;	// 上次请求的渲染是否完成
        private ArrayList<Runnable> mEventQueue = new ArrayList<Runnable>();
        
        // End of member variables protected by the sGLThreadManager monitor.
        private Renderer mRenderer;
        private EglHelper mEglHelper;		// 归当前线程所有
    }
    
    static class LogWriter extends Writer 
    {
        @Override public void close() 
        {
            flushBuilder();
        }

        @Override public void flush() 
        {
            flushBuilder();
        }

        @Override public void write(char[] buf, int offset, int count) 
        {
            for(int i = 0; i < count; i++) 
            {
                char c = buf[offset + i];
                if ( c == '\n') 
                {
                    flushBuilder();
                }
                else 
                {
                    mBuilder.append(c);
                }
            }
        }

        private void flushBuilder() 
        {
            if (mBuilder.length() > 0) 
            {
                Log.v("GLSurfaceView", mBuilder.toString());
                mBuilder.delete(0, mBuilder.length());
            }
        }

        private StringBuilder mBuilder = new StringBuilder();
    }

    private void checkRenderThreadState() 
    {
        if (mGLThread != null) 
        {
            throw new IllegalStateException("setRenderer has already been called for this instance.");
        }
    }

    private static class GLThreadManager 
    {
        private static String TAG = "GLThreadManager";

        public synchronized void threadExiting(GLThread thread) 
        {
            if(LOG_THREADS) 
            {
                Log.i("GLThread", "exiting tid=" +  thread.getId());
            }
            thread.mExited = true;
            if(mEglOwner == thread) 
            {
                mEglOwner = null;
            }
            notifyAll();
        }

        /*
         * Tries once to acquire the right to use an EGL
         * context. Does not block. Requires that we are already
         * in the sGLThreadManager monitor when this is called.
         *
         * @return true if the right to use an EGL context was acquired.
         */
        public boolean tryAcquireEglContextLocked(GLThread thread) 
        {
            if(mEglOwner == thread || mEglOwner == null) 
            {
                mEglOwner = thread;
                notifyAll();
                return true;
            }
            checkGLESVersion();
            if(mMultipleGLESContextsAllowed) 
            {
                return true;
            }
            return false;
        }

        /*
         * Releases the EGL context. Requires that we are already in the
         * sGLThreadManager monitor when this is called.
         */
        public void releaseEglContextLocked(GLThread thread)
        {
            if(mEglOwner == thread) 
            {
                mEglOwner = null;
            }
            notifyAll();
        }

        public synchronized boolean shouldReleaseEGLContextWhenPausing() 
        {
            // Release the EGL context when pausing even if
            // the hardware supports multiple EGL contexts.
            // Otherwise the device could run out of EGL contexts.
        	return true;
        	
        	// Note: 加入多EGLContext支持,注意上面的Warning!
        //  return !mMultipleGLESContextsAllowed;
        }
        
        private void checkGLESVersion() 
        {
            if(!mGLESVersionCheckComplete) 
            {
            	mMultipleGLESContextsAllowed = false;
                mGLESVersion = kGLES_10;//SystemProperties.getInt("ro.opengles.version", ConfigurationInfo.GL_ES_VERSION_UNDEFINED);
                if(mGLESVersion >= kGLES_20)
                {
                    mMultipleGLESContextsAllowed = true;
                }
                
                if(LOG_SURFACE) 
                {
                    Log.w(TAG, "checkGLESVersion mGLESVersion =" +
                            " " + mGLESVersion + " mMultipleGLESContextsAllowed = " + mMultipleGLESContextsAllowed);
                }
                mGLESVersionCheckComplete = true;
            }
        }

        public synchronized void checkGLDriver(GL10 gl) 
        {
            if(!mGLESDriverCheckComplete) 
            {
                checkGLESVersion();
                
                if(mGLESVersion < kGLES_20) 
                {
                    String renderer = gl.glGetString(GL10.GL_RENDERER);
                    mMultipleGLESContextsAllowed = !(renderer.startsWith("Q3Dimension MSM7500 ") ||
                    								 renderer.startsWith("Q3Dimension MSM7225"));
                    if(LOG_SURFACE) 
                    {
                        Log.w(TAG, "checkGLDriver renderer = \"" + renderer + "\" multipleContextsAllowed = "
                            + mMultipleGLESContextsAllowed);
                    }
                    notifyAll();
                }
                mGLESDriverCheckComplete = true;
            }
        }
        
        private boolean mGLESVersionCheckComplete;
        private int mGLESVersion;
        private boolean mGLESDriverCheckComplete;
        private boolean mMultipleGLESContextsAllowed;
        private static final int kGLES_10 = 0x10000;
        private static final int kGLES_20 = 0x20000;
        private GLThread mEglOwner;//  拥有EGL的GLThread
    }

    private static final GLThreadManager sGLThreadManager = new GLThreadManager();
    private boolean mSizeChanged = true; // 窗口大小是否发生了变化 

    private GLThread mGLThread;				// 渲染线程
    private EGLConfigChooser mEGLConfigChooser;	// EGL配置选择器
    private EGLContextFactory mEGLContextFactory;// EGL Context工厂
    private EGLWindowSurfaceFactory mEGLWindowSurfaceFactory;// 窗口Surface工厂
    private GLWrapper mGLWrapper;	// OpenGL浅包装
    private int mEGLContextClientVersion;
}
