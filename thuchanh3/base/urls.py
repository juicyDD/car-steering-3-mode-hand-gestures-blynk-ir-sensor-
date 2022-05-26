from django.urls import path
from . import views
urlpatterns =[
    path('',views.home,name='home'),
    path('video_feed/', views.video_feed, name="video-feed"),
    path('refresh-card/', views.refreshcard, name="refresh-card")
]