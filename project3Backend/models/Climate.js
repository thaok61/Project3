const mongoose = require('mongoose');

const ClimateSchema = new mongoose.Schema({
    site: {
        type: String,
        required: true,
    },
    type: {
        type: String,
        required: true,
    },
    value: {
        type: String,
        required: true,
    },
    date: {
        type: Date,
        default: Date.now
    }
})

module.exports = Climate = mongoose.model('climate', ClimateSchema);