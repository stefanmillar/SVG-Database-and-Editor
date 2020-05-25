'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');
var bodyParser = require('body-parser');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));
app.use(express.json());
app.use(bodyParser.json()); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true }));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

//Shared library
var svgparse = ffi.Library('./libsvgparse', {
  'getSVG': ['string', ['string']],
  'getDesc': ['string', ['string']],
  'getTitle': ['string', ['string']],
  'getJSONRects': ['string', ['string']],
  'getJSONCircles': ['string', ['string']],
  'getJSONPaths': ['string', ['string']],
  'getJSONGroups': ['string', ['string']],
  'editAttribute': ['string', ['string', 'string', 'string', 'string']],
  'editTitle': ['string', ['string', 'string']],
  'editDescription': ['string', ['string', 'string']],
  'addShape': ['string', ['string', 'string']],
  'scaleShape': ['string', ['string', 'string', 'string']],
  'addSVG': ['string', ['string']]
});

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

//Get uploads information
app.get('/uploads', function(req, res){
  var names = fs.readdirSync('uploads/');
  var sizes = [];
  var getSVG = [];
  names.forEach(filesizes);

  function filesizes(item) {
    var stats = fs.statSync("uploads/" + item);
    var size = stats["size"];
    sizes.push(size);
  }

  for(var i = 0; i < names.length; i++)
  {
    var jsonname = svgparse.getSVG("uploads/" + names[i])
    getSVG.push(JSON.parse(jsonname));
  }

  var SVGs = [];

  for(var i = 0; i < names.length; i++)
  {
    var SVG = {};
    SVG["name"] = names[i];
    SVG["size"] = sizes[i];
    SVGs.push(Object.assign({}, SVG, getSVG[i]));
  }

  console.log(SVGs);
  if(names.length != 0)
    res.send(SVGs);
  else
  {
    console.log("No files uploaded");
    res.send(SVGs);
  }
});

//Get the filesize of svg in uploads folder
app.get('/filesize', function(req, res){
  var stats = fs.statSync("uploads/" + req.query.fileName);
  var size = stats["size"];
  res.send({theSize: size});
})

//Get title of SVG
app.get('/title', function(req, res){
  var title = {}; 
  title["title"] = svgparse.getTitle("uploads/" + req.query.filename);
  if(title.title == "undefined")
    res.status(500).send({error: 'invalid filename'});
  else
    res.send(title);
})

//Get description of SVG
app.get('/description', function(req, res){
  var desc = {}; 
  desc["desc"] = svgparse.getDesc("uploads/" + req.query.filename);
  if(desc.desc == "undefined")
    res.status(500).send({error: 'invalid filename'});
  else
    res.send(desc);
})

//Get rectangle list
app.get('/rects', function(req, res) {
  var string;
  var rects = [];
  string = svgparse.getJSONRects("uploads/" + req.query.filename);
  rects = JSON.parse(string);

  if(string == "undefined")
    res.status(500).send({error: 'invalid filename'});
  else
    res.send(rects);
})

//Get circle list
app.get('/circles', function(req, res) {
  var string;
  var circles = [];
  string = svgparse.getJSONCircles("uploads/" + req.query.filename);
  circles = JSON.parse(string);

  if(string == "undefined")
    res.status(500).send({error: 'invalid filename'});
  else
    res.send(circles);
})

//Get path list
app.get('/paths', function(req, res) {
  var string;
  var paths = [];
  string = svgparse.getJSONPaths("uploads/" + req.query.filename);
  paths = JSON.parse(string);

  if(string == "undefined")
    res.status(500).send({error: 'invalid filename'});
  else
    res.send(paths);
})

//Get group list
app.get('/groups', function(req, res) {
  var string;
  var groups = [];
  string = svgparse.getJSONGroups("uploads/" + req.query.filename);
  groups = JSON.parse(string);

  if(string == "undefined")
    res.status(500).send({error: 'invalid filename'});
  else
    res.send(groups);
})

//Edit or add an attribute
app.post('/edit', function(req, res) {
  var status = svgparse.editAttribute("uploads/" + req.body.filename, req.body.component, req.body.name, req.body.value);
  if(status == "success")
    res.send("Success editing attribute");
  else
    res.status(500);
})

//Edit title
app.post('/editTitle', function(req, res) {
  console.log(req.body);
  var status = svgparse.editTitle("uploads/" + req.body.filename, req.body.title);
  console.log(status);
  if(status == "success")
    res.send("Success editing title");
  else
    res.status(500).send({error: 'Failure'});
})

//Edit description
app.post('/editDescription', function(req, res) {
  console.log(req.body);
  var status = svgparse.editDescription("uploads/" + req.body.filename, req.body.desc);
  console.log(status);
  if(status == "success")
    res.send("Success editing description");
  else
    res.status(500).send({error: 'Failure'});
})

//Add shape
app.get('/addShape', function(req, res) {
  console.log(req.query);
  var status = svgparse.addShape("uploads/" + req.query.filename, req.query.ashape);
  console.log(status);
  if(status == "success")
    res.send("Success adding shape");
  else
    res.status(500).send({error: 'Failure'});
})

//Add svg
app.post('/addSVG', function(req, res) {
  console.log(req.body);
  var status = svgparse.addSVG("uploads/" + req.body.filename);
  console.log(status);
  if(status == "success")
    res.send("Success adding shape");
  else
    res.status(500).send({error: 'Failure'});
})

//Scale shape
app.post('/scaleShape', function(req, res) {
  console.log(req.body);
  var status = svgparse.scaleShape("uploads/" + req.body.filename, req.body.component, req.body.size);
  console.log(status);
  if(status == "success")
    res.send("Success scaling shape");
  else
    res.status(500).send({error: 'Failure'});
})

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);