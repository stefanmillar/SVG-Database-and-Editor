// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
// On page-load
$.ajax({
    type: 'get',
    dataType: 'json',
    url: '/uploads',
    success: function (data) {
        if(data.length > 5)
            document.getElementsByTagName("div")[0].setAttribute("class", "scrollit");
        else if(data.length == 0)
        {
            $('#fileLog').append("<tr class=\"log\"><td>No Files<td></td>");
        }
        console.log(data);
           
        data.forEach(addSVG);
        data.forEach(addDropDownSVGs);
    },
    fail: function(error) {
        // Non-200 return, do something with error
        console.log(error); 
    }
});

//Change file log when file is uploaded
$('#uploadForm').submit(function(e) {
    updateFileLog(e);
});

//Change SVG view panel to selected SVG
$('#ImageDropdown').change(function(e){
    updateTable(e);
});

//Edit or add an attribute
$("#editForm").submit(function(e){        
    e.preventDefault();

    var attr = {
        filename: $("#ImageDropdown option:selected").text(),
        component: $("#componentDrop option:selected").text(),
        name: $('#editName').val(),
        value: $('#editValue').val()
    }

    if(attr.filename == "Select Image")
        alert("Select an image");
    else if((attr.component.includes("Rectangle") == false) && (attr.component.includes("Circle") == false) 
    && (attr.component.includes("Path") == false) && (attr.component.includes("Group") == false))
        alert("No valid components");
    else
    $.ajax({
        type: 'post',
        url: '/edit',
        data: attr,
        success: function(e) {
            console.log(status);
            updateTable(e);
            updateFileLog(e);
        },
        error: function(e) {
            console.log(e);
        }
    });
});

//Edit Title
$('#editTitleForm').submit(function(e) {
    e.preventDefault();

    var etitle = {
        filename: $("#ImageDropdown option:selected").text(),
        title: $('#SVGtitle').val()
    }

    console.log(etitle.title);
    if(etitle.filename == "Select Image")
        alert("Select an image");
    else
    {
        $.ajax({
            type: 'post',
            url: '/editTitle',
            datatype: 'json',
            data: etitle,
            success: function(e) {
                console.log("Title edited");
            },
            error: function(e) {
                console.log("Error posting title");
            }
        });
        updateTable(e);
    }
});

//Edit Description
$('#editDescForm').submit(function(e) {
    e.preventDefault();

    var edesc = {
        filename: $("#ImageDropdown option:selected").text(),
        desc: $('#SVGdesc').val()
    }

    if(edesc.filename == "Select Image")
        alert("Select an image");
    else
    {
        $.ajax({
            type: 'post',
            url: '/editDescription',
            datatype: 'json',
            data: edesc,
            success: function(e) {
                console.log("Description edited");
            },
            error: function(e) {
                console.log("Error posting description");
            }
        });
        updateTable(e);
    }
});

//Add a shape
$('#addShape').click(function(e) {
    var shape = {
        filename: $("#ImageDropdown option:selected").text(),
        ashape: $('#Shape').val()
    }
    if(shape.filename == "Select Image")
        alert("Select an image");
    else
    {
        $.ajax({
            type: 'get',
            url: '/addShape',
            datatype: 'json',
            data: shape,
            success: function(e) {
                console.log("Shape added");
                updateTable(e);
                updateFileLog(e);
            },
            error: function(e) {
                console.log("Error posting shape");
            }
        });
    }
});

//Add new SVG
$('#submitNewSVG').submit(function(e) {
    e.preventDefault();

    var file = {
        filename: $('#newSVG').val()
    }
    if(file.filename.substring(file.filename.length - 4, file.filename.length) != ".svg")
        alert("Must end with .svg");
    else
    {
        $.ajax({
            type: 'post',
            url: '/addSVG',
            datatype: 'json',
            data: file,
            success: function(e) {
                console.log("File added");
            },
            error: function(e) {
                console.log("Error posting file");
            }
        });
        updateFileLog(e);
    }
});

//Scale the svg
$('#scaleForm').submit(function(e) {
    e.preventDefault();

    var scale = {
        component: $("#componentDrop option:selected").text(),
        filename: $("#ImageDropdown option:selected").text(),
        size: $('#Scale').val()
    }
    if((scale.component.includes("Rectangle") == false) && (scale.component.includes("Circle") == false))
        alert("Must choose a rectangle or circle");
    else
    {
        $.ajax({
            type: 'post',
            url: '/scaleShape',
            datatype: 'json',
            data: scale,
            success: function(e) {
                console.log("Component scaled");
                updateTable(e);
                updateFileLog(e);
            },
            error: function(e) {
                alert("Must be between 0 and 10");
            }
        });
    }
});

//Add image names to dropdown image selection
function addDropDownSVGs(item) {
    $('#ImageDropdown').append("<option value=\"" + item.name + "\">" + item.name + "</option>");
}

//Add images to file log table
function addSVG(item) {
    $('#fileLog').append("<tr class=\"log\"><td><img src=\"" + item.name 
    + "\" width=\"200\"></td><td><a href=\"" + item.name 
    + "\">" + item.name 
    + "</a></td><td>" + Math.round(item.size / 1000) 
    + "kb</td><td>" + item.numRect + "</td><td>" + item.numCirc 
    + "</td><td>" + item.numPaths 
    + "</td><td>" + item.numGroups + "</td></tr>");
}

//Update file log
function updateFileLog(e) {
    $(".log").remove();
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/uploads',
        success: function (data) {
            //If more then 5 files make file log scrollable
            if(data.length > 5)
                document.getElementsByTagName("div")[0].setAttribute("class", "scrollit");

            var selected = $("#ImageDropdown option:selected").text();

            $('#ImageDropdown').empty();
            
            data.forEach(addSVG);
            data.forEach(addDropDownSVGs);

            $("#ImageDropdown option[value=\"" + selected + "\"]").prop('selected', 'selected');
        },
        fail: function(error) {
            console.log(error); 
        }
    });
}

function updateTable(e) {
    var image = $("#ImageDropdown option:selected").text();

    //Clear previous information
    $('#ImageView').empty();
    $('#desc').empty();
    $('#title').empty();
    $('#componentDrop').empty();
    $(".component").remove();

    $('#ImageView').append("<td colspan=\"6\" align=\"center\" height=\"800\"><img id=\"theImage\" src=\"" + image + "\" width=\"800\"></td>");

    $('#theImage').load(image);
    console.log("Updated");

    //Get title from server
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/title',
        data: {
            filename: image
        },
        success: function(data){
            console.log(data);
            $('#title').append("Title:<br>" + data.title);
        },
        fail: function(error){
            console.log(error);
        }
    });
    //Get description from server
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/description',
        data: {
            filename: image
        },
        success: function(data){
            console.log(data);
            $('#desc').append("Description:<br>" + data.desc);
        },
        fail: function(error){
            console.log(error);
        }
    });
    //Get rectangles from server
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/rects',
        data: {
            filename: image
        },
        success: function(data){
            console.log(data);
            for(let i = 0; i < data.length; i++)
            {

                $('#components').append("<tr class=\"component\"><td colspan=\"2\">Rectangle " + (i + 1) 
                + "</td><td colspan=\"2\">From upper left corner: x = " + data[i].x + data[i].units 
                + "  y = " + data[i].y + data[i].units 
                + "<br>width = " + data[i].w + data[i].units 
                + "  height = " + data[i].h + data[i].units + "</td><td colspan=\"2\">" 
                + data[i].numAttr + "</td></tr>");

                $('#componentDrop').append("<option class=\"rectangle\">Rectangle " + (i + 1) + "</option>");
            }
        },
        fail: function(error){
            console.log(error);
        }
    });
    //Get circles from server
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/circles',
        data: {
            filename: image
        },
        success: function(data){
            console.log(data);
            for(let i = 0; i < data.length; i++)
            {
                $('#components').append("<tr class=\"component\"><td colspan=\"2\">Circle " + (i + 1) 
                + "</td><td colspan=\"2\">From upper left corner: cx = " + data[i].cx + data[i].units 
                + "  cy = " + data[i].cy + data[i].units 
                + "<br>r = " + data[i].r + data[i].units + "</td><td colspan=\"2\">" 
                + data[i].numAttr + "</td></tr>");

                $('#componentDrop').append("<option class=\"circle\">Circle " + (i + 1) + "</option>");
            }
        },
        fail: function(error){
            console.log(error);
        }
    });
    //Get paths from server
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/paths',
        data: {
            filename: image
        },
        success: function(data){
            console.log(data);
            for(let i = 0; i < data.length; i++)
            {
                $('#components').append("<tr class=\"component\"><td colspan=\"2\">Path " + (i + 1) 
                + "</td><td colspan=\"2\">data: " + data[i].d
                + "</td><td colspan=\"2\">"
                + data[i].numAttr + "</td></tr>");

                $('#componentDrop').append("<option class=\"path\">Path " + (i + 1) + "</option>");
            }
        },
        fail: function(error){
            console.log(error);
        }
    });
    //Get groups from server
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/groups',
        data: {
            filename: image
        },
        success: function(data){
            console.log(data);
            for(let i = 0; i < data.length; i++)
            {
                $('#components').append("<tr class=\"component\"><td colspan=\"2\">Group " + (i + 1) 
                + "</td><td colspan=\"2\">Children: " + data[i].children
                + "</td><td colspan=\"2\">"
                + data[i].numAttr + "</td></tr>");

                $('#componentDrop').append("<option class=\"group\">Group " + (i + 1) + "</option>");
            }
        },
        fail: function(error){
            console.log(error);
        }
    });
}
});