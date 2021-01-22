const mongooose = require('mongoose');
const config = require('config');
const db = config.get('mongoURL');

const connectDB = async () => {
    try {
        await mongooose.connect(db, {
            useNewUrlParser: true,
            useUnifiedTopology: true,
            useCreateIndex: true
        });

        console.log('MongoDB Connected...');
    } catch(err) {
        console.log(err.message);

        process.exit(1);
    }
}

module.exports = connectDB;