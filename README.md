# SocketLab

POSIX socket programlama öğrenmek için C++17 lab projesi.  
Winsock bağımlılıkları tamamen kaldırıldı; Linux üzerinde doğrudan veya Docker ile çalışır.

---

## Proje Yapısı

```
SocketLab/
├── src/
│   ├── socket.h / socket.cpp        # Temel POSIX socket sarmalayıcı (referans)
│   ├── server.h / server.cpp        # TCP sunucu sınıfı
│   ├── client.h / client.cpp        # TCP istemci sınıfı
│   ├── server_main.cpp              # Sunucu giriş noktası
│   └── client_main.cpp              # İstemci giriş noktası
├── Makefile
├── Dockerfile
├── docker-compose.yml
└── README.md
```

---

## Yerel Derleme (Linux)

```bash
make          # server ve client ikili dosyalarını build/ klasörüne üretir
make server   # sadece sunucu
make client   # sadece istemci
make clean    # build/ klasörünü siler
```

Çalıştırma (iki ayrı terminal):

```bash
./build/server
./build/client localhost   # varsayılan host: "server" (Docker için)
```

---

## Docker ile Çalıştırma

### İlk kurulum (imajları oluştur)

```bash
docker compose build
```

### Sunucu + istemciyi aynı anda başlat

```bash
docker compose up
```

- `socketlab-server` → port 8080 üzerinde dinler  
- `socketlab-client` → Docker ağı üzerinden `server:8080`'e bağlanır

### Sadece sunucuyu arka planda başlat, istemciyi interaktif çalıştır

```bash
docker compose up -d server
docker compose run --rm client ./client server
```

### Farklı bir hedefe bağlan

```bash
docker compose run --rm client ./client <host>
```

### Durdurma ve temizleme

```bash
docker compose down
docker compose down --rmi all   # imajları da sil
```

---

## Sınıflar

### `Server`

| Metot | Açıklama |
|---|---|
| `Server(port, connections, type)` | Soket oluştur, SO_REUSEADDR ayarla, bind et |
| `accept_connections()` | listen + tek istemci kabul et |
| `send_bytes(data)` | Kabul edilen istemciye veri gönder |
| `set_blocking_type(type)` | Blocking / Non-blocking modu değiştir |

### `Client`

| Metot | Açıklama |
|---|---|
| `Client(host, port, type)` | Soket oluştur, adres bilgisini hazırla |
| `connect_socket()` | `getaddrinfo` + `connect` |
| `send_line(data)` | Sunucuya veri gönder |
| `receive_until()` | Bağlantı kapanana kadar veri al |
| `receive_echo()` | İnteraktif gönder/al döngüsü |

### Blocking / Non-blocking

Her iki sınıfta da `TypeSocket::BlockingSocket` ve `TypeSocket::NonBlockingSocket`  
`fcntl(fd, F_SETFL, O_NONBLOCK)` ile ayarlanır.

---

## Öğrenme Yol Haritası

1. `src/socket.h` – temel POSIX socket API'ını incele  
2. `Server` + `server_main.cpp` – `bind` → `listen` → `accept` akışını takip et  
3. `Client` + `client_main.cpp` – `getaddrinfo` → `connect` akışını takip et  
4. `blocking_mode()` – `fcntl` ile non-blocking soket nasıl ayarlanır?  
5. `docker-compose.yml` – iki container'ın aynı ağda nasıl haberleştiğini gözlemle
