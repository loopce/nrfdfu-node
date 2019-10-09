{
  "targets": [
    {
      "target_name": "nrfdfunode",
      "sources": [ "<!@(ls -1 csrc/bluez/*.c)", "<!@(ls -1 csrc/*.c)", "<!@(ls -1 csrc/*.cc)" ],
      "link_settings": {
        "libraries": [
            "-ljson-c",
            "-lzip",
            "-lbluetooth"
        ],
        "include_dirs": [
            "/usr/include/json-c",
            "<!@(node -p \"require('node-addon-api').include\")",
            "./csrc"
        ],
      },
      "dependencies": [
          "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}
