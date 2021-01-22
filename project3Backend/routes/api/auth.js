const express = require('express');
const router = express.Router();
const bcrypt = require('bcryptjs')
const jwt = require('jsonwebtoken');
const auth = require('../../middleware/auth');
const config = require('config');

const { check, validationResult } = require('express-validator/check')
const User = require('../../models/User');

// @route   Get api/auth
// @desc    get auth user
// @access  Public
router.get(
    '/', 
    auth,
    async (req,res) => {
        try {
            const user = await User.findById(req.user.id).select('-password')
            res.json(user);
        } catch (error) {
            console.error(err.message);
            res.status(500).send('Server Error');
        }
    }
);

// @route   Post api/auth
// @desc    Authenticat user & get token 
// @access  Public
router.post(
    '/',
    [
        check('email', 'Please include a valid email').isEmail(),
        check('password', 'Password is required').exists()
    ], 
    async (req,res) => {
        const errors = validationResult(req);
        if(!errors.isEmpty()) {
            return res.status(400).json({ errors: errors.array() })
        }

        const { email, password } = req.body;

        try {
            
            let user = await User.findOne({ email: email });
            
            // See if user exists
            if(!user) {
                res.status(400).json({ errors: [ {msg: 'Invalid Email'} ] });
            }

            const isMatch = await bcrypt.compare(password, user.password);

            if (!isMatch) {
                res.status(400).json({ errors: [ {msg: 'Invalid Password'} ] });
            }

            const payload = {
                user: {
                    id: user.id
                }
            }
            // Return jsonwebtoken
            jwt.sign(
                payload, 
                config.get('jwtSecret'),
                { expiresIn: 360000 },
                (err, token) => {
                if(err) throw err;
                res.json({ token })
                }
            );
        } catch(err) {
            console.error(err.message);
            res.status(500).send('Server error')
        }    
    }
);
module.exports = router;