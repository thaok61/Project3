const express = require('express');
const router = express.Router();
const auth = require('../../middleware/auth');

const Climate = require('../../models/Climate');

// @route   Get api/data/humidity
// @desc    get auth user
// @access  Public
router.get(
    '/humidity', 
    auth,
    async (req,res) => {
        try {
            const humidities = await Climate.find({ type: "humidity" })
            if (!humidities) {
                res.json({ message: "No Data"}) 
            }
            res.json(humidities);
        } catch (error) {
            console.error(err.message);
            res.status(500).send('Server Error');
        }
    }
);

// @route   Get api/data/temperature
// @desc    get auth user
// @access  Public
router.get(
    '/temperature', 
    auth,
    async (req,res) => {
        try {
            const temperatures = await Climate.find({ type: "temperature" })
            if (!temperatures) {
                res.json({ message: "No Data"}) 
            }
            res.json(temperatures);
        } catch (error) {
            console.error(err.message);
            res.status(500).send('Server Error');
        }
    }
);

module.exports = router;