const express = require("express");
const app = express();
require("dotenv").config();
const cors = require("cors");
const bodyParser = require("body-parser");

const port = process.env.PORT || 3000;
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());
app.use(cors());
app.use(express.json());

//
app.get("/", (req, res) => {
  res.send("Hello World");
});
app.post("/speed", (req, res) => {
  // res.json({speed: "1000mbps"})
  // console.log(res)
  console.log("Data received:", req.body);
  res.send("Data received successfully");
});

app.get("/speed", (req, res) => {
  // res.json({speed: "1000mbps"})
  // console.log(res)
  console.log("Data received:get", req.body);
  res.send("Data received successfully get request");
});

// async (params) => {
//     const url="http://localhost:3100/speed"
//     const response = await fetch(url,{
//         method: "GET",
//         headers: {
//             "Content-Type": "application/json"
//         },
//         body: JSON.stringify({ })
//     })

// const result = await response.json()
// console.log(result)

// }

app.listen(port, () => {
  console.log(`Server is running at ${port}`);
});
