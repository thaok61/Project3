const express = require('express');
const cors = require('cors');
const connectDB = require('./config/db')
const mqttHandler = require('./config/mqttHandler')
const app = express();

// Connect Database
connectDB();

// Configure CORS
var corsOptions = {
    origin: 'http://localhost:8080',
    optionsSuccessStatus: 200 // For legacy browser support
}

app.use(cors(corsOptions));

// Init Middleware
app.use(express.json({ extended: false}));

app.get('/', (req, res) => res.send('API Running'));

//Init MQTT
var mqttClient = new mqttHandler();
mqttClient.connect();

//Define Routes
app.use('/api/users', require('./routes/api/users'));
app.use('/api/data', require('./routes/api/data'));
app.use('/api/auth', require('./routes/api/auth'));

const PORT = process.env.PORT || 5001;

app.listen(PORT, () => console.log(`Server started on port ${PORT}`));