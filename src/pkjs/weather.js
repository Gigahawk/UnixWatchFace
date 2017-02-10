var weather_debug = true;
var weather_initialized;

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  if(weather_debug)
    console.log("Weather: I - Found location, getting weather...");
  // Construct URL
  var url = 'https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(SELECT%20woeid%20FROM%20geo.places%20WHERE%20text%3D%22(' +
      pos.coords.latitude + '%2C' + pos.coords.longitude + ')%22)%20AND%20u%3D%27c%27&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys';

  // Send request to Yahoo
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.query.results.channel.item.condition.temp);
      

      // Conditions
      var conditions = json.query.results.channel.item.condition.text;
      
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

function init(debug){
  weather_debug = debug;
  
  if(weather_debug)
    console.log("Weather: I - Initializing weather module");
  
  weather_initialized = true;
  
}

module.exports.init = init;
module.exports.getWeather = getWeather;


