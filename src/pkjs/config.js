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
            "label": "DarkSky", 
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
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];