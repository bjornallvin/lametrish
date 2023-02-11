var express = require("express");
var router = express.Router();

/* GET home page. */
router.get("/", function(req, res, next) {
    res.render("index", { title: "Express" });
});

router.post("/show", function(req, res, next) {
    let grid = req.body.grid;
    for (x in grid) {
        grid[x] = grid[x].join(":");
    }
    var grid_string = "G:" + grid.join("::") + ";";
    console.log(grid_string);
    arduino.write(grid_string);
    res.json({ result: "ok" });
});

module.exports = router;
