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
      },
      {
        "type": "text",
        "id": "weather_fallback_output",
        "defaultValue": ""
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Quiet Time"
      },
      {
        "type": "checkboxgroup",
        "messageKey": "quiet_hours_disabled_features",
        "label": "Disabled Features",
        "defaultValue": [true, true, false],
        "options": ["Weather", "Phone Battery", "Watch Battery"],
        "description": "Functions to disable when quiet time is enabled"
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
    "hidden": true,
    "items": [
      {
        "type": "heading",
        "id": "debug_header",
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