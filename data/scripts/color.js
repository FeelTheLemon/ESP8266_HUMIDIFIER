var colorPicker = new iro.ColorPicker("#color-picker-container", {
	//width: 290,
	//height: 360,
	//handleRadius: 8,
	handleUrl: null,
	// handleUrl: "#test",
	//handleOrigin: {y: 0, x: 0},
	color: "#f00",
	borderWidth: 2,
	padding: 8,
	wheelLightness: true,
	wheelAngle: 270,
	wheelDirection: 'anticlockwise',
	layout: [
		{
			component: iro.ui.Wheel,
			options: {
			}
		},
		{
			component: iro.ui.Slider,
			options: {
			}
		},
		{
			component: iro.ui.Slider,
			options: {
				sliderType: 'hue'
			}
		},
		{
			component: iro.ui.Slider,
			options: {
				sliderType: 'saturation'
			}
		},
		{
			component: iro.ui.Slider,
			options: {
				sliderType: 'brightness'
			}
		},
	]
})

colorPicker.timer = null;

function updateColors() {
	let args = {
		color_r: colorPicker.color.rgb.r,
		color_g: colorPicker.color.rgb.g,
		color_b: colorPicker.color.rgb.b
	}

	let URI = "";

	for (let key in args) {
		if (URI != "") {
			URI += "&";
		}
		URI += key + "=" + encodeURIComponent(args[key]);
	}

	let xh = new XMLHttpRequest()
	xh.open("GET", "/set?" + URI, true)
	xh.send(null)
}

function loadColors() {
	let xh = new XMLHttpRequest()
	xh.onreadystatechange = function(){
		if (xh.readyState == 4){
			if(xh.status == 200) {
				let res = JSON.parse(xh.responseText)
				colorPicker.color.rgb = {
					r: res.color.r,
					g: res.color.g,
					b: res.color.b
				}
			}
		}
	}
	xh.open("GET", "/get", true)
	xh.send(null)
}

loadColors()

colorPicker.on('color:change', function() {
	clearTimeout(colorPicker.timer);
	colorPicker.timer = setTimeout(updateColors, 50);
})
