module.exports = [
  {
    "type": "heading",
    "defaultValue": "UnixWatchFace Settings"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Weather"
      },
      {
        "type": "select",
        "messageKey": "weather_provider",
        "defaultValue": "darksky",
        "label": "Weather Provider",
        "options": [
          { 
            "label": "Yahoo", 
            "value": "yahoo" 
          },
          { 
            "label": "Dark Sky", 
            "value": "darksky" 
          }
        ]
      },
      {
        "type": "select",
        "messageKey": "weather_units",
        "defaultValue": "si",
        "label": "Units",
        "options": [
          { 
            "label": "SI", 
            "value": "si" 
          },
          { 
            "label": "US", 
            "value": "us" 
          }
        ]
      },
      {
        "type": "slider",
        "messageKey": "weather_interval",
        "defaultValue": 60,
        "label": "Update Interval",
        "description": "Number of minutes between a weather update (lower values will consume more power)",
        "min": 10,
        "max": 240,
        "step": 10
      },
      {
        "type": "toggle",
        "messageKey": "weather_fail_indicator",
        "capabilities": ["COLOR"],
        "label": "Show Weather Update Fail",
        "defaultValue": true,
        "description": "Every time a weather update fails the color of the weather text will turn red"
      },
      {
        "type": "slider",
        "messageKey": "weather_retries",
        "defaultValue": 3,
        "label": "Retries",
        "description": "Number of times to retry a failed weather fetch",
        "min": 0,
        "max": 10,
        "step": 1
      },
      {
        "type": "input",
        "messageKey": "weather_fallback",
        "label": "Fallback Location",
        "defaultValue": "",
        "description": "Location to use in case GPS can't find your location."
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Communication"
      },
      {
        "type": "toggle",
        "messageKey": "quiet_hours",
        "label": "Quiet Hours",
        "defaultValue": true,
        "description": "During quiet hours certain features can be disabled to reduce battery consumption"
      },
      {
        "type": "input",
        "messageKey": "quiet_hours_start",
        "label": "Quiet Hours Start",
        "defaultValue": "11:59 PM",
        "attributes": {
          "type": "time"
        }
      },
      {
        "type": "input",
        "messageKey": "quiet_hours_end",
        "label": "Quiet Hours End",
        "defaultValue": "8:00 AM",
        "attributes": {
          "type": "time"
        }
      },
      {
        "type": "checkboxgroup",
        "messageKey": "quiet_hours_disabled_features",
        "label": "Disabled Features",
        "defaultValue": [true, true, false, false],
        "options": ["Weather", "Phone Battery", "Watch Battery", "Time"],
        "description": "Note: If time is disabled it will only update when the watch is shaken (uses accelerometer, may actually be detrimental to battery life)"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "API Keys"
      },
      {
        "type": "input",
        "messageKey": "darksky_api_key",
        "label": "Dark Sky API",
        "description": 'Required to use Dark Sky as a weather provider, get a key <a href="https://darksky.net/dev/register">here</a>'
      },
      {
        "type": "input",
        "messageKey": "google_api_key",
        "label": "Google Maps API",
        "description": 'Required for the fallback location to work, get a key <a href="https://developers.google.com/maps/documentation/geocoding/start#get-a-key">here</a>'
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Debug"
      },
      {
        "type": "text",
        "id": "debug_output",
        "defaultValue": ""
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];