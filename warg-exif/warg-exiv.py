import pyexiv2
import argparse
import os, os.path
import string
import csv

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('dir', metavar='IMG_DIR', help='Directory of the image files to parse')
parser.add_argument('csv', metavar='CSV_PATH', help='Path of the csv')

args = parser.parse_args()

times = []
for root, _, files in os.walk(args.dir):
   for f in files:
       fullpath = os.path.join(root, f)
       metadata = pyexiv2.ImageMetadata(fullpath)
       metadata.read()
       tag = metadata['Exif.Image.DateTime']
       time = tag.raw_value[tag.raw_value.index(' ')+1:].replace(':','')
       times.append((f, time))
times = sorted(times)
print "fileName,lat,lon,altitude,heading"
if 1:
    with open(args.csv, 'rb') as csvfile:
            reader = csv.reader(csvfile, delimiter=',')
            rows = list(reader)
            lat = rows[0].index('lat')
            lon = rows[0].index('lon')
            time = rows[0].index('time')
            altitude = rows[0].index('altitude')
            heading = rows[0].index('heading')
            current = times.pop(0)
            while 1:
                for row in rows[1:]:
                    time_s = row[time].replace(" ", "")
                    if time_s.find(".") != -1:
                        time_s = time_s[:time_s.index(".")]
                    if time_s == '' or time_s == '0' or len(time_s) < 5:
                        continue
                    time_i = int(time_s)
                    if int(time_s[len(time_s)-2:]) >= 53:
                       time_i -= 53
                    else:
                    if int(time_s[len(time_s)-4:len(time_s)-2]) != 0:
                        time_i += (-100 + 7)
                    else:
                    if int(time_s[:len(time_s)-5]) != 0:
                         time_i += (-10000 + 5907)
                    else:
                        time_i += 235907
                    if time_i >= current:
                        print(time_s+ "," + row[lat] + "," + row[lon] + "," + row[altitude] + "," + row[heading])
                        quit()
                        current = times.pop(0)
                current = times.pop(0)

