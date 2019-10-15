const nrfdfunode = require("./index");

const result = nrfdfunode.uploadFirmware(process.argv[2], process.argv[3], (result) => {
    console.log("Result was: " + result);
});
