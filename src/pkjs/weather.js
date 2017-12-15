var weather_debug = true;
var has_gps = false;
var weather_initialized;
var weather_provider;
var weather_units;
var weather_fallback;
var weather_retries;
var weather_fail_indicator;

var fallback_pos;

// TODO: Pull these from settings or smth
var darksky_key;
var yahoo_key; // Yea I have no idea why this works without a key
var google_key;

var xhrRequest = function (url, type, success, failure, s_params, f_params) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    success(this.responseText,s_params);
  };
  xhr.onerror = function (e){
    if(failure instanceof Function)
      failure(this.statusText,f_params);
    else
      console.log("XHR error: " + this.statusText);
  };
  xhr.open(type, url);
  xhr.send();
};

function initFallback(callback){
  var url = 'https://maps.googleapis.com/maps/api/geocode/json?address=' + 
    weather_fallback + 
    '&key=' + google_key;
  
  if(weather_debug){
    console.log("Weather: I - Getting location from fallback using:");
    console.log(url);
  }

  xhrRequest(url, 'GET',
    function(responseText) {
      var json = JSON.parse(responseText);
      fallback_pos = {
        "coords": {
          "latitude": json.results[0].geometry.location.lat,
          "longitude": json.results[0].geometry.location.lng
        }
      };
      
      if(weather_debug){
        console.log("Fallback Pos:");
        console.log(json.results[0].formatted_address);
        console.log(fallback_pos.coords.latitude);
        console.log(fallback_pos.coords.longitude);
      }
      if(callback instanceof Function)
        callback(fallback_pos);
    });
}

function getURL(pos) {
  if(weather_debug)
    console.log("Weather: I - Getting URL for " + weather_provider);
  
  var url;
  
  switch(weather_provider){
    case "darksky":
      url = 'https://api.darksky.net/forecast/' +
          darksky_key + '/' + 
          pos.coords.latitude + ',' + 
          pos.coords.longitude + '?units=' + 
          weather_units + '&exclude=[minutely,hourly,daily,flags]';
      if(weather_debug)
        console.log("Weather: I - URL for darksky is " + url);
      break;
    case "yahoo":
      url = 'https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(SELECT%20woeid%20FROM%20geo.places%20WHERE%20text%3D%22(' + 
          pos.coords.latitude + '%2C' + 
          pos.coords.longitude + ')%22)%20AND%20u%3D%27' + 
          (weather_units == 'si'? 'c': 'f') + '%27&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys';
      if(weather_debug)
        console.log("Weather: I - URL for yahoo is " + url);
      break;
    default:
      
      url = 'https://api.darksky.net/forecast/ad2c5eab8490dd926f3802c093c04324/' + 
          pos.coords.latitude + ',' + 
          pos.coords.longitude + 
          '?units=si&exclude=[minutely,hourly,daily,flags]';
      if(weather_debug)
        console.log("Weather: W - Unknown weather provider " + weather_provider + ", resetting to darksky as fallbacl");
      
      weather_provider = "darksky";
  }
  
  return url;
}

function sendWeather(msg){
  
  var dictionary = {
    'WEATHER': msg,
    'WEATHER_GPS': has_gps
  };
  Pebble.sendAppMessage(dictionary,
    function(e) {
      if(weather_debug)
        console.log("Weather: I - Weather info sent to Pebble successfully!");
    },
    function(e) {
      if(weather_debug)
        console.log("Weather: E - Could not send weather info to Pebble!");
    });
}

function sendFail(){
  if(!weather_fail_indicator)
    return;
  var dictionary = {
    'WEATHER_FAILED':1
  };
  Pebble.sendAppMessage(dictionary,
    function(e) {
      if(weather_debug)
        console.log("Weather: I - Weather failure sent to Pebble successfully!");
    },
    function(e) {
      if(weather_debug)
        console.log("Weather: E - Could not send weather failure to Pebble!");
    });
}

function parseWeather(responseText) {
  // responseText contains a JSON object with weather info
  var json = JSON.parse(responseText);
  
  var temperature;
  var conditions;
  
  // Get weather data from json response
  switch(weather_provider){
    case "darksky":
      temperature = Math.round(json.currently.temperature);
      conditions = json.currently.summary;
      break;
    case "yahoo":
      temperature = Math.round(json.query.results.channel.item.condition.temp);
      conditions = json.query.results.channel.item.condition.text;
      break; 
  }
  
  var weather_string = "" + temperature + (weather_units == "si" ? 'C':'F') +  ', ' + conditions;
  if(weather_debug){
    console.log('Temperature is ' + temperature);
    console.log('Conditions are ' + conditions);
    console.log(weather_string);
  }
  if(weather_string)
    sendWeather(weather_string);
  else
    sendFail();
}

function locationSuccess(pos) {
  if(weather_debug)
    console.log("Weather: I - Found location, getting weather...");
  
  // Only gps coords should have speed
  if(pos.coords.speed != undefined)
    has_gps = true;
  
  var params = [weather_retries];
  fetchWeather(pos, params);
}

function fetchWeather(pos, params){
  // Check number of retries left
  if(params[0] <= 0){
    console.log("Weather: E - Weather could not be fetched");
    sendFail();
    return;
  }
  params[0]--;
  
  // Construct URL
  var url = getURL(pos);
  
  // Send request
  xhrRequest(url, 'GET', parseWeather, fetchWeather, [], params);
}

function locationError(err) {
  if(weather_debug)
    console.log("Weather: E - Error requesting location, using fallback...");
  
  has_gps = false;
  
  if(!fallback_pos){
    initFallback(locationSuccess);
  } else {
    locationSuccess(fallback_pos);
  }
  
}

function getWeather() {
  if(weather_debug)
    console.log("Weather: I - Getting weather");
  if((typeof weather_initialized === "undefined") || (weather_initialized === false)) {
    if(weather_debug)
      console.log("Weather: E - Weather isn't initialized");
    return;
  }
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 30000, maximumAge: 60000}
  );
}

function getProvider() {
  return weather_provider;
}

function setProvider(provider) {
  weather_provider = provider;
}

function getUnits(){
  return weather_units;
}
function setUnits(units) {
  weather_units = units;
}

function getFallback(){
  return weather_fallback;
}
function setFallback(fallback){
  weather_fallback = fallback;
  fallback_pos = undefined;
  initFallback();
}

function setFailIndicator(){
  return weather_fail_indicator;
}
function getFailIndicator(failIndicator){
  weather_fail_indicator = failIndicator;
}
function setDarkSkyKey(key){
  darksky_key = key;
}
function setGoogleKey(key){
  google_key = key;
}

function init(settings){
  if(weather_debug)
    console.log("Weather: I - Initializing weather module");
  
  weather_provider = settings.weather_provider;
  weather_units = settings.weather_units;
  weather_fallback = settings.weather_fallback;
  weather_retries = settings.weather_retries;
  weather_fail_indicator = settings.weather_fail_indicator;
  
  setDarkSkyKey(settings.darksky_api_key);
  setGoogleKey(settings.google_api_key);
  
  // Attempt to initialize fallback ahead of time
  initFallback();
  
  weather_initialized = true;
}

module.exports.init = init;
module.exports.getWeather = getWeather;
module.exports.getProvider = getProvider;
module.exports.setProvider = setProvider;
module.exports.getUnits = getUnits;
module.exports.setUnits = setUnits;
module.exports.getFallback = getFallback;
module.exports.setFallback = setFallback;
module.exports.getFailIndicator = getFailIndicator;
module.exports.setFailIndicator = setFailIndicator;

