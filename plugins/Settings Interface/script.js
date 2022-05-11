function delay(time) {
    return new Promise(resolve => setTimeout(resolve, time));
}

function reload() {
    intermission.style.visibility = "visible"            
    
    delay(15000).then(() => { 
        console.log("Wait");
        start(); 
        
    });

}
function start() {
    let intermission = document.getElementById("intermission")

    writeField3();
    writeField4();            intermission.style.visibility = "hidden"
}
const writeField3 = async () => {
    const response = await fetch('https://api.thingspeak.com/channels/1710398/fields/3.json?api_key=SYQ3Z9VTRH7BWV7E&results=1');
    const myJson = await response.json();
    const val = myJson.feeds[0].field3
    document.getElementById("tempgoal").value = val
    document.getElementById("curr-tempgoal").innerHTML = val
    document.getElementById("tempval").innerHTML = val
}
const writeField4 = async () => {
    const response = await fetch('https://api.thingspeak.com/channels/1710398/fields/4.json?api_key=SYQ3Z9VTRH7BWV7E&results=1');
    const myJson = await response.json();
    const val = myJson.feeds[0].field4
    document.getElementById("humidgoal").value = val
    document.getElementById("curr-humidgoal").innerHTML = val + "%"
    document.getElementById("humidval").innerHTML = val + "%"
}