import requests

BOT_TOKEN = "5012916931:AAHmjC3K-_QYic0tYYvpVGAMqLi4USVuERY"
ID_CHAT_CARLOS = 5032892089
ID_CHAT_JAUME = 1908996766

mensaje = "Fin impresion"

url = f"https://api.telegram.org/bot{BOT_TOKEN}/sendMessage?chat_id={ID_CHAT_CARLOS}&text={mensaje}"
print(requests.get(url).json()) # Enviar mensaje

url = f"https://api.telegram.org/bot{BOT_TOKEN}/sendMessage?chat_id={ID_CHAT_JAUME}&text={mensaje}"
print(requests.get(url).json()) # Enviar mensaje


url = f"https://api.telegram.org/bot{BOT_TOKEN}/getUpdates"
print(requests.get(url).json())