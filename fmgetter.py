#coding: utf-8
import urllib
import urllib2
import json
import cookielib


class FmGetter(object):
	def __init__(self):
		self._url = "http://www.douban.com/j/app/radio/people?app_name=radio_desktop_win&version=100&channel=1&sid=undefined&type=n"
		cookie = cookielib.CookieJar()
		self._opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cookie))
	
	def get(self):
		response = self._opener.open(self._url)
		data = response.read()
		songs = json.loads(data)
		# print len(songs["song"])
		# for x in songs['song']:
		# 	print x['url']
		# song = songs["song"][1]
		# print song
		# print unicode(str_result,"utf-8") 
		return songs["song"]
		# return dict(
		# 			url = song["url"],
		# 			artist = song["artist"],
		# 			title = song["title"],
		# 			length = song["length"]
		# 		)
	
	
if __name__ == "__main__":
	fm = FmGetter()
	print fm.get()
	print fm.get()
	print fm.get()
