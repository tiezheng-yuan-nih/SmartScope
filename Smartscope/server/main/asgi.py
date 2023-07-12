"""
ASGI config for autoscreenServer project.

It exposes the ASGI callable as a module-level variable named ``application``.

For more information on this file, see
https://docs.djangoproject.com/en/3.0/howto/deployment/asgi/
"""

import os
from django.conf import settings
import django


django.setup()

from channels.routing import ProtocolTypeRouter
from django.core.asgi import get_asgi_application
<<<<<<< HEAD
from Smartscope.server.websocket.routing import application
=======
from channels.security.websocket import AllowedHostsOriginValidator, OriginValidator
from channels.auth import AuthMiddlewareStack
from Smartscope.server.websocket.routing import router

application = ProtocolTypeRouter({
    "http": get_asgi_application(),
    "websocket": AllowedHostsOriginValidator(
        AuthMiddlewareStack(
            router
        )
    )
})
>>>>>>> 65a9bcc (clarify asgi application and fix websocket)
