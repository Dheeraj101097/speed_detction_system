const express = require("express")
const app = express()
require("dotenv").config();
const cors = require("cors");
const bodyParser = require("body-parser");

const port = process.env.PORT || 8000;

app.use(bodyParser.json());
app.use(cors());
app.use(express.json());

//
app.get("/",(req,res)=>{
    res.send("Hello World")
})
app.post("/speed",(req,res)=>{
    // res.json({speed: "1000mbps"})
    const data = req.body
    console.log(data)
    // console.log(res)
})

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

app.listen(port,()=>{
    console.log(`Server is running at ${port}`)
})