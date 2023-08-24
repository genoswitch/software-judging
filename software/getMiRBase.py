import requests

with open("mature.fa", 'wb') as f:
    f.write(requests.get("https://mirbase.org/download/mature.fa").content)
