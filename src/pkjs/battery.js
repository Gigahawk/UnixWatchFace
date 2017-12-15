var batt_debug = true;
var batt_initialized = false;
var battery;

function push_success(){
  if(batt_debug)
    console.log("Battery: I - Push success");
}

function push_failure(){
  if(batt_debug)
    console.log("Battery: E - Push failure");
}

function push(){
  if(batt_debug)
    console.log("Battery: I - Pushing battery state to watch");
  if(!batt_initialized){
    if(batt_debug)
      console.log("Battery: E - Battery modules are not initialized");
    return;
  }
  
  var dictionary = 
      {
        'PHONE_BATTERY': battery.level *100,
        'PHONE_CHARGING': battery.charging ? 1 : 0
      };
  
  Pebble.sendAppMessage(dictionary,push_success, push_failure);    
}

function setBatteryExists(exists){
  var dictionary = 
      {
        'PHONE_BATTERY_EXISTS': exists
      };
  Pebble.sendAppMessage(dictionary, push_success, push_failure);
}

function init_success(batteryManager)
{
  if(batt_debug)
    console.log("Battery: I - Battery modules initialized");
  
  battery = batteryManager;
  batt_initialized = true;
  
  push();
}

function init_failure(){
  if(batt_debug)
    console.log("Battery: E - Could not initialize battery module");
}

function init(debug){
  batt_debug = debug;
  
  if(batt_debug)
    console.log("Battery: I - Iniitializing battery modules");
  
  if("getBattery" in navigator) {
    navigator.getBattery().then(function(battery){
      battery.addEventListener('chargingchange', function(){
        push();
      });
      battery.addEventListener('levelchange', function(){
        push();
      });
      setBatteryExists(true);
    });
    navigator.getBattery().then(init_success, init_failure);
  } else {
    if(batt_debug)
      console.log("Battery: E - Phone does not support Battery API");
    setBatteryExists(false);
  }  
}

module.exports.init= init;
module.exports.push= push;