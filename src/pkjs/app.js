var com_debug = false;
var com_initialized = false;
var battery = require('./battery');
var weather = require('./weather');

function ready_success(){
  if(com_debug)
    console.log("Com: I - Push success");
}

function ready_failure(){
  if(com_debug)
    console.log("Com: E - Push failure");
}


function ready(){
  if(com_debug)
    console.log("com: I - Comms are ready");
  if(!com_initialized){
    if(com_debug)
      console.log("com: E - Comms could not be initialized");
    return;
  }
  
  var dictionary = 
      {
        'READY': 1,
      };
  
  Pebble.sendAppMessage(dictionary,ready_success, ready_failure);    
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    if(com_debug)
      console.log('PebbleKit JS ready!, running inits');
    com_init(com_debug);
    battery.init(com_debug);
    weather.init(com_debug);
    ready();
  }
);

function com_init(debug){
  if(debug)
    console.log("Com: I - Initializing Comms");
  
  com_initialized = false;
  
  Pebble.addEventListener('appmessage', function(e){
    if(debug){
      console.log("Com: I - Recieved AppMessage");
    }
    if(com_initialized){
      if ('WEATHER_TEMPERATURE' in e.payload){
        if(debug)
          console.log("Com: I - Weather info requested");
        weather.getWeather();
      }
      if ('PHONE_BATTERY' in e.payload) {
        if(debug){
          console.log("Com: I - Battery info requested");
          console.log("Com: W - Battery info does not need to be explicitly requested.");
        }
        battery.push();
        
      }
      if ('END' in e.payload) {
        if(debug)
          console.log("Com: I - End of message found");
      } else {
        if(debug)
          console.log("Com: W - End of message not found");
      }
    } else if (debug){
      console.log("Com: E - Comms are not initialized");
    }
  });
  
  com_initialized = true;
  if(debug)
    console.log("Com: I - Comms initialized");
}