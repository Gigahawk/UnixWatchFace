var weather_debug = true;
var weather_initialized;
var weather_provider;
var weather_units;

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getURL(pos) {
  if(weather_debug)
    console.log("Weather: I - Getting URL for " + weather_provider);
  
  var url;
  
  switch(weather_provider){
    case "darksky":
      url = 'https://api.darksky.net/forecast/ad2c5eab8490dd926f3802c093c04324/' + 
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

function locationSuccess(pos) {
  if(weather_debug)
    console.log("Weather: I - Found location, getting weather...");
  // Construct URL
  var url = getURL(pos);

  // Send request to Yahoo
  xhrRequest(url, 'GET', 
    function(responseText) {
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
      
      if(weather_debug){
        console.log('Temperature is ' + temperature);
        console.log('Conditions are ' + conditions);
      }
      
      // Assemble dictionary using our keys
      var dictionary = {
        'WEATHER_TEMPERATURE': temperature,
        'WEATHER_CONDITIONS': conditions
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          if(weather_debug)
            console.log("Weather: I - Weather info sent to Pebble successfully!");
        },
        function(e) {
          if(weather_debug)
            console.log("Weather: E - Could not send weather info to Pebble!");
        }
      );
    }      
  );
}

function locationError(err) {
  if(weather_debug)
    console.log("Weather: E - Error requesting location!");
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
    {timeout: 15000, maximumAge: 60000}
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

function init(debug){
  weather_debug = debug;
  
  if(weather_debug)
    console.log("Weather: I - Initializing weather module");
  
  weather_initialized = true;
  
}

module.exports.init = init;
module.exports.getWeather = getWeather;
module.exports.getProvider = getProvider;
module.exports.setProvider = setProvider;
module.exports.getUnits = getUnits;
module.exports.setUnits = setUnits;


