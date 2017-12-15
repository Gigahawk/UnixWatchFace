module.exports = function(minified) {
  var clayConfig = this;
//   var _ = minified._;
  var $ = minified.$;
//   var HTML = minified.HTML;

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
    var debug_log = clayConfig.getItemById('debug_output');
    var weather_provider = clayConfig.getItemByMessageKey('weather_provider');
    var weather_fallback = clayConfig.getItemByMessageKey('weather_fallback');
    var darksky_key = clayConfig.getItemByMessageKey('darksky_api_key');
    var google_key = clayConfig.getItemByMessageKey('google_api_key');
    
    var log = function(text) {
      var curr_text = debug_log.get();
      curr_text = curr_text + '<br>' + text;
      debug_log.set(curr_text);
    };
    
    var xhrRequest = function (url, type, success, failure) {
      var xhr = new XMLHttpRequest();
      xhr.onload = function () {
        success(this.responseText);
      };
      xhr.onerror = function (e){
        if(failure instanceof Function)
          failure(this.statusText);
        else
        log("XHR error: " + this.statusText);
      };
      xhr.open(type, url);
      xhr.send();
    };
    
    log("Clay: I - Setting up api checks");
    
    var check_darksky = function(){
      var key = darksky_key.get();
      var url = "https://api.darksky.net/forecast/" + key + "/37.8267,-122.4233";
      log("Clay: I - Checking Dark Sky using:");
      log(url);
      xhrRequest(url,'GET', function(responseText){
        log(responseText);
      }, function(statusText){
        log(statusText);
      });
//       $.request('get', url).then(function(result){
//         log(result);
//         weather_provider.enable();
//       }).error(function(status, statusText, responseText){
//         log("Clay: E - Couldn't access darksky");
//         log(status);
//         log(statusText);
//         log($.toJSON(responseText));
//         weather_provider.set("yahoo");
//         weather_provider.disable();
//       });
    };
    
    var check_google = function() {
      var key = google_key.get();
      var url = "https://maps.googleapis.com/maps/api/geocode/json?address=1600+Amphitheatre+Parkway,+Mountain+View,+CA&key=" + key;
      $.request('get', url).then(function(result){
        var json = $.parseJSON(result);
        if(json.status == "OK"){
          weather_fallback.enable();
        }else{
          weather_fallback.set("");
          weather_fallback.disable();
        }
      }).error(function(status, statusText, responseText){
        weather_fallback.set("");
        weather_fallback.disable();
      }); 
    };
    
    check_darksky();
    check_google();
    
    //Check darksky api key for validity
    darksky_key.on('change', check_darksky);
    
    //Check google api key for validity
    google_key.on('change', check_google);
    
  });
};