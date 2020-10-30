# Mapping New York City Open Street Map (OSM) with Cartopy
# This code uses a spoofing algorithm to avoid bounceback from OSM servers
#
import matplotlib.pyplot as plt 
import numpy as np
import cartopy.crs as ccrs
import cartopy.io.img_tiles as cimgt
import io
from urllib.request import urlopen, Request
from PIL import Image

def image_spoof(self, tile): # this function pretends not to be a Python script
    url = self._image_url(tile) # get the url of the street map API
    req = Request(url) # start request
    req.add_header('User-agent','Anaconda 3') # add user agent to request
    fh = urlopen(req) 
    im_data = io.BytesIO(fh.read()) # get image
    fh.close() # close url
    img = Image.open(im_data) # open image with PIL
    img = img.convert(self.desired_tile_form) # set image format
    return img, self.tileextent(tile), 'lower' # reformat for cartopy

cimgt.OSM.get_image = image_spoof # reformat web request for street map spoofing
osm_img = cimgt.OSM() # spoofed, downloaded street map

fig = plt.figure(figsize=(12,9)) # open matplotlib figure

ax1 = plt.subplot(231, projection=osm_img.crs) # project using coordinate reference system (CRS) of street map

#center_pt = [40.713, -74.0135] # lat/lon of One World Trade Center in NY

center_pt = [50.7313, -3.5170] # lat/lon of nearby

zoom = 0.00075 # for zooming out of center point
#zoom = 0.00500 # for zooming out of center point
extent = [center_pt[1]-(zoom*2.0),center_pt[1]+(zoom*2.0),center_pt[0]-zoom,center_pt[0]+zoom] # adjust to zoom
ax1.set_extent(extent) # set extents

scale = np.ceil(-np.sqrt(2)*np.log(np.divide(zoom,350.0))) # empirical solve for scale based on zoom
scale = (scale<20) and scale or 19 # scale cannot be larger than 19
print('scale', scale)
ax1.add_image(osm_img, int(scale)) # add OSM with zoom specification
# NOTE: zoom specifications should be selected based on extent:
# -- 2     = coarse image, select for worldwide or continental scales
# -- 4-6   = medium coarseness, select for countries and larger states
# -- 6-10  = medium fineness, select for smaller states, regions, and cities
# -- 10-12 = fine image, select for city boundaries and zip codes
# -- 14+   = extremely fine image, select for roads, blocks, buildings


print('load')
fname = '/home/nawal/data/Max/GPS_Logger/data/Data04_5595d59_walk_2_rounds.csv'
with open(fname, 'r') as f:
    lines = f.read().split('\n')
lines = list(filter(lambda x: len(x) > 0, lines))
time_arr = []
date_arr = []
lat_arr = []
lon_arr = []
alt_arr = []
speed_arr = []
bearing_arr = []
AccX_arr = []
AccY_arr = []
AccZ_arr = []
GyroX_arr = []
GyroY_arr = []
GyroZ_arr = []
print('parse')
for i in range(1, len(lines)):
    time, date, lat, lon,  alt, speed, bearing, AccX,AccY,AccZ,GyroX,GyroY,GyroZ = lines[i].split(',')
    time_arr.append(time)
    date_arr.append(date)
    lat_arr.append(float(lat))
    lon_arr.append(float(lon))
    alt_arr.append(float(alt))
    speed_arr.append(float(speed))
    bearing_arr.append(float(bearing))
    AccX_arr.append(float(AccX))
    AccY_arr.append(float(AccY))
    AccZ_arr.append(float(AccZ))
    GyroX_arr.append(float(GyroX))
    GyroY_arr.append(float(GyroY))
    GyroZ_arr.append(float(GyroZ))

lat_arr = np.array(lat_arr)
lon_arr = np.array(lon_arr)
alt_arr = np.array(alt_arr)
speed_arr = np.array(speed_arr)
bearing_arr = np.array(bearing_arr)
AccX_arr = np.array(AccX_arr)
AccY_arr = np.array(AccY_arr)
AccZ_arr = np.array(AccZ_arr)
GyroX_arr = np.array(GyroX_arr)
GyroY_arr = np.array(GyroY_arr)
GyroZ_arr = np.array(GyroZ_arr)



# Plot my locations

print('plot')

print(list(zip(lon_arr, lat_arr)))

ax1.plot(lon_arr, lat_arr, markersize=5,marker='o',linestyle='',color='red',transform=ccrs.PlateCarree())

print('show')

#lats = np.array([50.731282, 50.730629, 50.730904])
#lons = np.array([-3.517934, -3.516810, -3.515966])
#ax1.plot(lons, lats, markersize=5,marker='o',linestyle='',color='blue',transform=ccrs.PlateCarree())


#print(lon_arr-lons[0])
#print(lat_arr-lats[0])
plt.subplot(232)
plt.plot(alt_arr)
plt.title('alt (m)')

plt.subplot(233)
plt.plot(speed_arr)
plt.title('speed')


plt.subplot(234)
plt.plot(bearing_arr)
plt.title('bearing')


plt.subplot(235)
plt.plot(AccX_arr, label='acc x')
plt.plot(AccY_arr, label='acc y')
plt.plot(AccZ_arr, label='acc z')
plt.legend()
plt.title('Acc')

plt.subplot(236)
plt.plot(GyroX_arr, label='gyro x')
plt.plot(GyroY_arr, label='gyro y')
plt.plot(GyroZ_arr, label='gyro z')
plt.legend()
plt.title('Gyro')

print(AccX)
print(AccY)
print(AccZ)

print(GyroX)
print(GyroY)
print(GyroZ)








plt.show() # show the plot
