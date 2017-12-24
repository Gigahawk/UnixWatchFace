module.exports = function(minified) {
  var clayConfig = this;
//   var _ = minified._;
  var $ = minified.$;
//   var HTML = minified.HTML;
  
  // CORS prevents requests from working inside Clay for some reason
  var nocors = "https://cors-anywhere.herokuapp.com/";
  
  // Keep text here in case we need to revert
  var fallback_desc = "Location to use in case GPS can't find your location.";

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
    var debug_header = clayConfig.getItemById('debug_header');
    var debug_log = clayConfig.getItemById('debug_output');
    debug_header.hide();
    debug_log.hide();
    var weather_provider = clayConfig.getItemByMessageKey('weather_provider');
    var weather_fallback = clayConfig.getItemByMessageKey('weather_fallback');
    var weather_fallback_output = clayConfig.getItemById('weather_fallback_output');
    var darksky_key = clayConfig.getItemByMessageKey('darksky_api_key');
    var google_key = clayConfig.getItemByMessageKey('google_api_key');
    
    var log = function(text) {
      var curr_text = debug_log.get();
      curr_text = curr_text + '<br>' + text;
      debug_log.set(curr_text);
    };
     
    log("Clay: I - Setting up api checks");
    
    var check_darksky = function(){
      var key = darksky_key.get();
      var url = nocors + "https://api.darksky.net/forecast/" + key + "/37.8267,-122.4233";
      log("Clay: I - Checking Dark Sky using:");
      log(url);

      $.request('get', url).then(function(result){
//         log(result);
        weather_provider.enable();
      }).error(function(status, statusText, responseText){
        weather_provider.set("yahoo");
        weather_provider.disable();
        
        log("Clay: E - Couldn't access darksky");
        log(status);
        log(statusText);
        try{
          log($.toJSON(responseText));
        } 
        catch(err) {
          log("No JSON recieved");
        }
      });
    };
    
    var check_google = function() {
      var key = google_key.get();
      var url = nocors + "https://maps.googleapis.com/maps/api/geocode/json?address=1600+Amphitheatre+Parkway,+Mountain+View,+CA&key=" + key;
      log("Clay: I - Checking Google Maps using:");
      log(url);
      
      $.request('get', url).then(function(result){
        log(result);
        var json = JSON.parse(result);
        log(json.status);
        if(json.status == "OK"){
          weather_fallback.enable();
        }else{
          weather_fallback.set("");
          weather_fallback.disable();
        }
      }).error(function(status, statusText, responseText){
        weather_fallback.set("");
        weather_fallback.disable();
        
        log("Clay: E - Couldn't access Google Maps");
        log(status);
        log(statusText);
        try{
          log($.toJSON(responseText));
        } 
        catch(err) {
          log("No JSON recieved");
        }
      }); 
    };
    
    var geocode = function() {
      var query = weather_fallback.get();
      var key = google_key.get();
      var url = nocors + 'https://maps.googleapis.com/maps/api/geocode/json?address=' + query + '&key=' + key;
      
      log("Clay: I - Geocoding using:");
      log(url);
      
      $.request('get', url).then(function(result){
        log(result);
        var json = JSON.parse(result);
        var formatted_address = json.results[0].formatted_address;

        if(formatted_address){
          weather_fallback_output.set("Fallback location: " + formatted_address);
        } else {
          weather_fallback_output.set("");
        }
      }).error(function(status, statusText, responseText){
        log("Clay: E - Couldn't access Google Maps");
        log(status);
        log(statusText);
        try{
          log($.toJSON(responseText));
        } 
        catch(err) {
          log("No JSON recieved");
        }
        weather_fallback_output.set("Error");
      }); 
      
    };
    
    weather_fallback.description = "test";
    
    check_darksky();
    check_google();
    geocode();
    
    //Check darksky api key for validity
    darksky_key.on('change', check_darksky);
    
    //Check google api key for validity
    google_key.on('change', check_google);
    
    weather_fallback.on('change', geocode);
    
  });
};