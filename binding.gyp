{
    "targets": [
        {
            "target_name": "message-height-twitch",
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "sources": [
                "./src/index.cpp"
            ],
            # In order for this to work, we must compile coreruncommon.cpp using the -FPIC flag. Expect some "cannot link shared lib" errors otherwise.
            "libraries": ["<(module_root_dir)/origin/c-interop/libcoreruncommon.a"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS'],
        }
    ]
}
