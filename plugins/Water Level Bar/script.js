function delay(time) {
    return new Promise(resolve => setTimeout(resolve, time));
}

function update() {
    updateBar();

    delay(15000).then(() => { 
        update(); 
    });
}

const updateBar = async () => {
    const response = await fetch('https://api.thingspeak.com/channels/1710398/fields/6.json?api_key=SYQ3Z9VTRH7BWV7E&results=1');
    const myJson = await response.json();
    let val = (myJson.feeds[0].field6)
    document.getElementById("full-bar").style.visibility = (val >= 500) ? "visible" : "hidden"
    document.getElementById("half-bar").style.visibility = (val >= 300) ? "visible" : "hidden"
}