import express from "express";
import {Locations} from "./locations.js";

const Api = express.Router();
Api.use(express.json());
Api.use("/locations", Locations);

export { Api }
