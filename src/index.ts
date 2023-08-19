import express from "express";
import {Api} from "./apis/index.js";

const port = process.env.PORT || 3000;

const server = express();
server.use("/api", Api);

server.listen(port, () => {
    console.log(`Server listening on ${port}`);
});
