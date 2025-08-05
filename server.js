// server.js - Node.js server to serve HTML and static images
const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();
const port = 8080;

const STATIC_DIR = path.join(__dirname, 'static');

// Serve static image and wasm folder
app.use('/static', express.static(STATIC_DIR));
app.use(express.static(__dirname));

// Route for main HTML page
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'index.html'));
});

// List static folder contents for JS
app.get('/static', (req, res) => {
  fs.readdir(STATIC_DIR, (err, files) => {
    if (err) return res.status(500).send('Could not list images');
    const images = files.filter(f => f.match(/\.(jpg|jpeg|png|webp)$/i));
    const list = images.map(name => `<a href=\"${name}\">${name}</a>`).join('<br>');
    res.send(`<html><body>${list}</body></html>`);
  });
});

app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});
