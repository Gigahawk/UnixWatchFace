var battery = require('./battery');
var weather = require('./weather');
var Clay = require('pebble-clay');
var messageKeys = require('message_keys');
var clayConfig = require('./config');
// var customClay = require('./custom-clay');

var com_debug = true;
var clay_debug = true;
var com_initialized = false;

var clay = new Clay(clayConfig, null, { autoHandleEvents: false });

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    if(com_debug)
      console.log('PebbleKit JS ready!, running inits');
    com_init(com_debug);
    battery.init(com_debug);
    
    // Get settings from clay
    var clay_settings = JSON.parse(localStorage.getItem('clay-settings'));
    weather.init(clay_settings);
    ready();
  }
);

  Pebble.addEventListener('showConfiguration', function(e) {
  if(clay_debug)
    console.log('Opening settings');
  Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
  if(clay_debug){
    console.log('Closing settings');
    console.log(JSON.stringify(e));
  }
  
  if (e && !e.response) {
    return;
  }

  // Get values without converting to message_keys
  var dict = clay.getSettings(e.response);
  
  // Handle settings that should stay local to the phone
  var weather_provider_key = messageKeys.weather_provider;
  var weather_fallback_key = messageKeys.weather_fallback;
  var weather_units_key = messageKeys.weather_units;
  var weather_fail_indicator_key = messageKeys.weather_fail_indicator;
  var darksky_api_key_key = messageKeys.darksky_api_key;
  var google_api_key_key = messageKeys.google_api_key;

  var weather_provider = dict[weather_provider_key];
  var weather_fallback = dict[weather_fallback_key];
  var weather_units = dict[weather_units_key];
  var weather_fail_indicator = dict[weather_fail_indicator_key];
  var darksky_api_key = dict[darksky_api_key_key];
  var google_api_key = dict[google_api_key_key];
  
  if(clay_debug){
    console.log("Handling local settings");
    console.log("weather_provider is " + weather_provider);
    console.log("weather_fallback is " + weather_fallback);
    console.log("weather_units is " + weather_units);
    console.log("weather_fail_indicator is " + weather_fail_indicator);
  }
  
  if(weather_provider){
    weather.setProvider(weather_provider);
    delete dict[weather_provider_key];
  }
  if(weather_fallback){
    weather.setFallback(weather_fallback);
    delete dict[weather_fallback_key];
  }
  if(weather_units){
    weather.setUnits(weather_units);
    delete dict[weather_units_key];
  }
  if(weather_fail_indicator){
    weather.setFailIndicator(weather_fail_indicator);
    //Don't delete, the phone should have access to this as well
  }
  
  if(darksky_api_key){
    weather.setDarkSkyKey(darksky_api_key);
    delete dict[darksky_api_key_key];
  }
  if(google_api_key){
    weather.setGoogleKey(google_api_key);
    delete dict[google_api_key_key];
  }

  // Send settings values to watch side
  Pebble.sendAppMessage(dict, function(e) {
    console.log('Sent config data to Pebble');
  }, function(e) {
    console.log('Failed to send config data!');
    console.log(JSON.stringify(e));
  });
});

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

function com_init(debug){
  if(debug)
    console.log("Com: I - Initializing Comms");
  
  com_initialized = false;
  
  // Handle incoming appmessages
  Pebble.addEventListener('appmessage', function(e){
    if(debug){
      console.log("Com: I - Recieved AppMessage");
    }
    if(com_initialized){
//       if ('weather_provider' in e.payload){
//         weather.setProvider(e.payload.weather_provider);
//         if(debug){
//           console.log("Com: I - Weather provider change requested");
//           console.log("Com: I - New weather provider is " + weather.getProvider());
//         }
//         // Update weather after provider change
//         weather.getWeather();
//       }
      
      if ('WEATHER' in e.payload){
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