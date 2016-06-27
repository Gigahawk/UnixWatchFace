var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

/** OPENWEATHER IS NICE BUT IT KINDA DOESNT WORK SOOO
function locationSuccess(pos) {
  // Construct URL
  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
      pos.coords.latitude + '&lon=' + pos.coords.longitude;

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp - 273.15);
      console.log('Temperature is ' + temperature);

      // Conditions
      var conditions = json.weather[0].main;      
      console.log('Conditions are ' + conditions);
      
      // Assemble dictionary using our keys
      var dictionary = {
        'KEY_TEMPERATURE': temperature,
        'KEY_CONDITIONS': conditions
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log('Weather info sent to Pebble successfully!');
        },
        function(e) {
          console.log('Error sending weather info to Pebble!');
        }
      );
    }      
  );
}
**/

/** WOEID lookup (obselete)
function locationSuccess(pos) {
  // Construct URL
  var urlwoeid = 'https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20geo.placefinder%20where%20text%3D%22' +
	pos.coords.latitude + '%2C' + pos.coords.longitude + '%22%20and%20gflags%3D%22R%22&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys';
	
	// Send request to Yahoo cause they fucking suck balls
  xhrRequest(urlwoeid, 'GET', 
    function(responseTextwoeid) {
      // responseTextwoeid contains a JSON object with woeid info
      var jsonwoeid = JSON.parse(responseTextwoeid);

      //parse woeid and push to console
      var woeid = jsonwoeid.results.Result.woeid;
      console.log('Yahoos bullshit is ' + woeid);
      
      // Assemble dictionary using our keys
      var dictionary = {
        'KEY_WOEID': woeid,
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log('Yahoos bullshit sent to Pebble successfully!');
        },
        function(e) {
          console.log('Yahoos being a fucking piece of shit right now');
        }
      );
    }      
  );
}
**/

function locationSuccess(pos) {
  // Construct URL
  var url = 'https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(SELECT%20woeid%20FROM%20geo.places%20WHERE%20text%3D%22(' +
      pos.coords.latitude + '%2C' + pos.coords.longitude + ')%22)%20AND%20u%3D%27c%27&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys';

  // Send request to Yahoos bullshit
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.query.results.channel.item.condition.temp);
      console.log('Temperature is ' + temperature);

      // Conditions
      var conditions = json.query.results.channel.item.condition.text;      
      console.log('Conditions are ' + conditions);
      
      // Assemble dictionary using our keys
      var dictionary = {
        'KEY_TEMPERATURE': temperature,
        'KEY_CONDITIONS': conditions
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log('Weather info sent to Pebble successfully!');
        },
        function(e) {
          console.log('Error sending weather info to Pebble!');
        }
      );
    }      
  );
}

function locationError(err) {
  console.log('Error requesting location!');
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    getWeather();
  }                     
);