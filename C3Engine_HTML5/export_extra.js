function _libiconv_open($tocode, $fromcode)
{
}

function _libiconv_close($icd)
{
}

function _libiconv($icd, $inbuf, $inbytesleft, $outbuf, $outbytesleft) 
{
}

Module["Runtime"] = Runtime;
Module["Runtime"]["stackAlloc"] = Runtime.stackAlloc;
Module["Runtime"]["staticAlloc"] = Runtime.staticAlloc;
Module["stackSave"] = Runtime.stackSave;
Module["stackRestore"] = Runtime.stackRestore;

Module["FS"]=FS;
Module["FS"]["createDataFile"] = FS.createDataFile;
Module["FS"]["createPath"] = FS.createPath;
Module["FS"]["createLazyFile"] = FS.createLazyFile;
Module["FS"]["forceLoadFile"] = FS.forceLoadFile;

Module["HEAPU8"] = HEAPU8;
Module["HEAPU16"] = HEAPU16;
Module["HEAPU32"] = HEAPU32;
