var colorPicker = new iro.ColorPicker('#picker');

var colorPicker2 = new iro.ColorPicker('#picker2');

const update = () => {
    var hex = colorPicker.color.hexString;
    var hex2 = colorPicker2.color.hexString;
    console.log(hex);
    console.log(hex2);
    const Http = new XMLHttpRequest();
    const url='https://yourwebsite?namecolor='.concat(hex.slice(1),'&backcolor=',hex2.slice(1));
    console.log(url);
    Http.open("GET", url);
    Http.send();
};

colorPicker.on('input:end', function(color) {
    update();
});

colorPicker2.on('input:end', function(color) {
    // log the current color as a HEX string
    update();
});
