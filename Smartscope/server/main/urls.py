"""
autoscreenServer URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.0/topics/http/urls/

Examples:

Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')

Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
    
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))

"""

from django.contrib import admin
from django.urls import path, include
from django.views.generic import RedirectView
from django.contrib.auth.views import LogoutView
from django.views.static import serve
from django.urls import re_path
from django.conf import settings
from django.conf.urls.static import static
from . import views


from . import forms

urlpatterns = [
    path('admin/', admin.site.urls),
    path('smartscope/', include('Smartscope.server.frontend.urls')),
    path('login/', views.MyLoginView.as_view(authentication_form=forms.UserLoginForm), name='login'),
    path('logout/', LogoutView.as_view()),
    path('', RedirectView.as_view(url="smartscope/browse")),
]
urlpatterns += static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)

# Api urls
# Wire up our API using automatic URL routing.
# Additionally, we include login URLs for the browsable API.
urlpatterns += [
    path('api/', include('Smartscope.server.api.urls')),
    path('api-auth/', include('rest_framework.urls', namespace='rest_framework'))
]
