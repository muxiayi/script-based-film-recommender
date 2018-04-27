var express = require('express');
var router = express.Router();

router.get('/users', function(req, res){
    var nums = {"result":true, "count":42};
    return res.json(nums);
});

module.exports = router;
