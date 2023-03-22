from flask import Flask, render_template, request, jsonify
import os
import requests
from datetime import datetime, timedelta, time

app = Flask(__name__)
cache_dict = {}
a = 9
def get_diff_curr(dotw, start_time):
    k = datetime.now().weekday()
    #format = '%Y-%m-%d %H:%M %p'
    # if(datetime.now().time < datetime.strptime(str(datetime.now().date()) + start_time, format)):
    curr_hour = int(str(datetime.now().time()).split(":")[0])
    class_hour = int(start_time.split(":")[0])
    class_hour %= 12
    m_ness = start_time.split(" ")[1]
    if (m_ness == "PM"):
        class_hour += 12
    diff = 100
    days = ['M','T','W','R','F','S','U']
    if(curr_hour > class_hour):
      for i in range(0,7):
        if(days[(i+1)%7] in dotw):
          print("h1")
          diff = min(diff,(i-k)%7+1)
    else:
      for i in range(0,7):
        if(days[i] in dotw):
          diff = min(diff,(i-k)%7+1)

    return diff*24 - curr_hour + class_hour + 1


def format_as_date(dotw, start_time):
    diff = 100
    k = datetime.now().weekday()
    curr_hour = int(str(datetime.now().time()).split(":")[0])
    class_hour = int(start_time.split(":")[0])
    class_hour %= 12
    m_ness = start_time.split(" ")[1]
    if (m_ness == "PM"):
        class_hour += 12
    days = ['M','T','W','R','F','S','U']
    if(curr_hour > class_hour):
      for i in range(0,7):
        if(days[(i+1)%7] in dotw):
          print("h1")
          diff = min(diff,(i-k)%7+1)
    else:
      for i in range(0,7):
        if(days[i] in dotw):
          diff = min(diff,(i-k)%7+1)
    print(dotw)
    print(curr_hour > class_hour)
    print(diff)
    curr_day = int(datetime.now().day) + diff
    class_time = [int(start_time.split(":")[0]), int(
        start_time.split(":")[1].split(" ")[0])]
    m_ness = start_time.split(" ")[1]
    class_time[0] %= 12
    if (m_ness == "PM"):
        class_time[0] += 12
    
    hour_str = str(class_time[0])
    if(class_time[0] < 10):
      hour_str = str(0)+hour_str
    min_str = str(class_time[1])
    if(class_time[1] < 10):
      min_str = str(0)+min_str
    month = str(int(datetime.now().month))
    if(len(month) == 1):
      month = str(0)+month
    return str(int(datetime.now().year)) + "-" + month + "-" + str(curr_day) + " " + hour_str+":"+min_str+":00"


# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")


# Route for "/weather" (middleware):
@app.route('/weather', methods=["POST"])
def POST_weather():
    try:
        course = request.form["course"]
        if(course in cache_dict):
          return cache_dict[course]
    except requests.exceptions.RequestException as e:
        return jsonify({"error": "Error processing request, please try again."}), 400
    course_subj = course[:2]
    num_start = len(course.rstrip('0123456789'))
    course_num = course[num_start:]
    print("HERE_0")
    print(num_start)
    if (num_start >= 4):
      print("HERE")
      if(course[:4] == "TEST"):
        print("ok what")
        course_subj = "TEST"
    params = {course_subj: course_num}
    tsl = "http://127.0.0.1:5000/"
    server_url = os.getenv('COURSES_MICROSERVICE_URL')
    try:
        course_data = requests.get(
            f'{server_url}/{course_subj}/{course_num}/').json()
        dotw = course_data["Days of Week"]
        start_time = course_data["Start Time"]
        print(start_time)
    except requests.exceptions.RequestException as e:
        return jsonify({"error": "No such course."}), 400
    except KeyError as e:
        return jsonify({"error": "No such course."}), 400
    except ValueError as e:
        return jsonify({"error": "No such course."}), 400
    try:
        weather_data = requests.get(
            "https://api.weather.gov/gridpoints/ILX/95,71/forecast/hourly").json()
    except requests.exceptions.RequestException as e:
        return jsonify({"error": "No Response from NWS api, please try again."}), 400
    # print(weather_data)

    class_time = get_diff_curr(dotw, start_time)
    date = format_as_date(dotw, start_time)
    date_forcast = format_as_date(dotw, start_time.split(
        ":")[0] + ":00 " + start_time.split(" ")[1])
    if (class_time > 144 or class_time < 0):
        cache = {"course": course_subj.upper() + " " + course_num,
                        "nextCourseMeeting": date,
                        "forecastTime": date_forcast,
                        "temperature": "forecast unavailable",
                        "shortForecast": "forecast unavailable"}
        cache_dict[course] = cache
        return jsonify(cache),200
    temperature = ""
    forecast = ""
    for p in weather_data["properties"]["periods"]:
        if (p["startTime"][:19] == date_forcast.replace(" ", "T")):
            temperature = p["temperature"]
            forecast = p["shortForecast"]
    else:
        cache =  {"course": course_subj.upper() + " " + course_num,
                        "nextCourseMeeting": date,
                        "forecastTime": date_forcast,
                        "temperature": temperature,
                        "shortForecast": forecast}
        cache_dict[course] = cache
        return jsonify(cache),200
    return jsonify({"error": "Not implemented."}), 500
    return "Not Implemented", 501


# Route for "/weatherCache" (middleware/backend):
@app.route('/weatherCache')
def get_cached_weather():
  # ...
  print(cache_dict)
  return jsonify(cache_dict),200
