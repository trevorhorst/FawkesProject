/* API Routes */
let XHR_TIMEOUT_MS = 5000;  // 5000 milliseconds before API gives up

// require("babel-polyfill");
// var baseRoute = window.location.href + 'api/command'
var baseRoute = window.location.href
var proxyRoute = window.location.href + 'proxy/'

function doFilePost(route, payload) {

    return new Promise(function(resolve, reject) {
        
        // We have a file list, not a singular file
        var formData = new FormData();
        payload.forEach(function(file) {
            formData.append('uploads[]', file, file.name)
        })

        var xhr = new XMLHttpRequest();
        xhr.open('POST', route);
        xhr.timeout = XHR_TIMEOUT_MS; // timeout (milliseconds)
        //xhr.setRequestHeader('Content-Type', 'multipart/form-data');
        xhr.onload = function() {
            if (xhr.status >= 200 && xhr.status < 300) {
                resolve(xhr.responseText);
            } else {
                console.log()
                reject(xhr.responseText);
            }
        };

        xhr.ontimeout = function(e) {
            console.log("XHR Timeout")
			reject(e);

        }

        xhr.onerror = function(e) {
            console.log(route, payload)
            console.log("XHR Error")
            reject(e);
        };

        xhr.send(formData);
    })
}

module.exports.sendFile = function(fileList) {
    doFilePost('http://localhost:3000/upload', fileList)
}

/**
 * @brief Sends a backwards compatible post request to the backend
 * @param route Destination URL
 * @param payload Data to send
 */
function doPost( route, payload ) {
    return new Promise( function( resolve, reject ) {
        // Create a new XHR
        var xhr = new XMLHttpRequest();
		xhr.open( 'POST', route );
		// Timeout in milliseconds
        xhr.timeout = XHR_TIMEOUT_MS;
        xhr.setRequestHeader( 
			'Content-Type', 'application/json; charset=UTF-8' );

        // Act on a successful XHR transaction hook
        xhr.onload = function( ) {
            // Do something with the retreived data
            if( xhr.status >= 200 && xhr.status < 300 ) {
                // We really only care about the response text
                resolve( JSON.parse( xhr.responseText ) );
            } else {
                reject( { 
                    status: xhr.status 
                    , statusText: xhr.statusText
                } );
            }
        };

        // Act on a timeout hook
        xhr.ontimeout = function( e ) {
            console.log( "XHR Timeout" )
            reject( e );
        }

        // Act on an error hook
        xhr.onerror = function( e ) {
            console.log( route, payload )
            console.log( "XHR Error" )
            reject( e );
        };

        xhr.send( JSON.stringify( payload ) );
    })
}

// Same as doPost, but always resolves (this circumvents Promise.all rejecting entire command when only one fails)
function doPostResolve(route, payload) {
    return new Promise(function(resolve, reject) {
        var xhr = new XMLHttpRequest();
        xhr.open('POST', route);
        xhr.timeout = XHR_TIMEOUT_MS; // timeout (milliseconds)
        xhr.setRequestHeader('Content-Type', 'application/json');
        xhr.onload = function() {
            if (xhr.status >= 200 && xhr.status < 300) {
                resolve(JSON.parse(xhr.responseText));
            } else {
                resolve(JSON.parse(xhr.responseText));
            }
        };

        xhr.ontimeout = function(e) {
            console.log("XHR Timeout")
            resolve({success: false, message: "XHR Timeout"});
        }

        xhr.onerror = function(e) {
            console.log("XHR Error")
            resolve({success: false, message: "XHR Error"});
        };

        xhr.send(JSON.stringify(payload));
    })
}

module.exports.proxyKey = null;
module.exports.setProxyKey = function(newKey)  {
    module.exports.proxyKey = newKey
}

// Accepts a single JSON radio command as parameter
// Returns a promise that resolves if command was succesful, and rejects otherwise
module.exports.sendCommand = function( payload ) {
    if( module.exports.proxyKey != null ) {
        var proxyPayload = {
            command: payload,
            radioKey: module.exports.proxyKey
        }
        return doPost( proxyRoute, proxyPayload );
    } else {
        return doPost( baseRoute, payload );
    }
}

module.exports.sendCommandToRadio = function(radio, payload) {
    var proxyPayload = {
        command: payload,
        radioKey: radio
    }
    return doPost(proxyRoute, proxyPayload);
}

module.exports.sendCommandListToRadio = function(radio, payloadList) {
    let promiseList = []
    payloadList.forEach(function(val, index, arr) {
        promiseList.push(module.exports.sendCommandToRadio(radio, val))
    })
    return Promise.all(promiseList);
}

// Accepts an array of JSON radio commands as a parameter
// Returns a promise that resolves if commands were succesful, and rejects otherwise
module.exports.sendCommandList = function(payloadList) {
    let promiseList = []
    payloadList.forEach(function(val, index, arr) {
        promiseList.push(module.exports.sendCommand(val))
    })
    return Promise.all(promiseList);
}

module.exports.getRadioList = function() {
    return doPost(window.location.href + 'getRadioList/')
}

// Always resolves
module.exports.sendCommandToRadioResolve = function(radio, payload) {
    var proxyPayload = {
        command: payload,
        radioKey: radio
    }
    return doPostResolve(proxyRoute, proxyPayload);
}

module.exports.removeRadio = function( key ) {
    var removeKey = {
        radioKey: key
    }
    return doPost(window.location.href + 'removeRadio/', removeKey);
}

module.exports.addRadio = function( key, radio ) {
    var addKey = {
        radioKey: key
        , radioDest: radio
    }
    return doPost(window.location.href + 'addRadio/', addKey);
}
