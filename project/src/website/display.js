// CSE 474
// Final Project

// This is the Javascript file for front-end display

"use strict";

(function() {
    
    const URLBASE = "https://cse474-yuxuanjiang.c9users.io/"
    let input_count = 0;

    window.onload = function() {
        $("input_count").innerHTML = input_count;
        $("fileInput").addEventListener("change", updateData, false);
        this.setInterval(updateCurrentTime, 1000);
    };

    // Display current time
    function updateCurrentTime() {
        let today = new Date();
        let hr = today.getHours();
        let mn = today.getMinutes();
        let sc = today.getSeconds();
        $("ctime").innerHTML = hr + ":" + mn + ":" + sc;
    }

    // Update data when the user selects a new file
    function updateData() {
        let file = this.files[0];
        let textType = /text.*/;
        if (file.type.match(textType)) {
            var reader = new FileReader();
            reader.onload = function(e) {
                let coords = reader.result;
                let xcoord = coords.substring(1, 4);
                let ycoord = coords.substring(5, 8);
                $("xcoord").innerHTML = xcoord;
                $("ycoord").innerHTML = ycoord;
                sendData(xcoord, ycoord);
            }
            reader.readAsText(file);	
        } else {
            alert("File not supported!");
        }
        input_count++;
        $("input_count").innerHTML = input_count;
    }
    
    // Ajax GET 
    // Send new coordinates to the server, get average coordinates from the server
    function sendData(xcoord, ycoord) {
        let url = URLBASE + "display.php?";
        url += "xcoord=";
        url += xcoord;
        url += "&ycoord="
        url += ycoord;

        fetch(url, {credentials: 'include'})
            .then(checkStatus)
            .then(JSON.parse)
            .then(loadAjaxResponse)
            .catch(handleError);
    }
    
    // Load xavg and yavg into HTML
    function loadAjaxResponse(response) {
        $("xavg").innerHTML = response["xcoord"];
        $("yavg").innerHTML = response["ycoord"];
    }

    function $(id) {
        return document.getElementById(id);
    }
    
    // Check Ajax promise status
    function checkStatus(response) {
        if (response.status >= 200 && response.status < 300) {
            return response.text();
        } else {
            return Promise.reject(new Error(response.status+": " + response.statusText));
        }
    }
    
    // Display Ajax error
    function handleError(errorMsg, statusCode) {
        console.log("Can not get coords! message " + errorMsg + " " + statusCode);
    }
})();