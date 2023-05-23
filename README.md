# Robot MONIBOT

Robot ini dirancang untuk memonitoring lokasi tambang dengan menggunakan sensor MQ-4, MQ-7, dan DHT22. Robot akan mengumpulkan data gas metana (CH4) dan karbon monoksida (CO) menggunakan sensor MQ-4 dan MQ-7, serta suhu dan kelembaban menggunakan sensor DHT22.

## Komponen Robot

1. Mikrokontroler: ESP32, ESP32CAM
2. Sensor MQ-4: Digunakan untuk mendeteksi konsentrasi gas metana (CH4)
3. Sensor MQ-7: Digunakan untuk mendeteksi konsentrasi karbon monoksida (CO)
4. Sensor DHT22: Digunakan untuk mengukur suhu dan kelembaban
5. Baterai 12V : Digunakan untuk menyuplai daya seluruh komponen
6. Rangkaian penurun tegangan 5V : Digunakan untuk menurunkan tegangan 12V ke 5V

## Cara Penggunaan

1. Pastikan mikrokontroler terhubung dengan sensor MQ-4, MQ-7, dan DHT22 secara tepat.
2. Sambungkan mikrokontroler ke sumber daya (misalnya baterai atau adaptor listrik).
3. Pastikan kode program untuk mikrokontroler sudah diunggah dengan benar.
4. Setelah robot dinyalakan, sensor-sensor akan mulai mengumpulkan data.
5. Data hasil monitoring dapat diakses melalui website monitoring MONIBOT
6. Pastikan untuk memantau hasil monitoring secara berkala dan mengambil tindakan jika terdeteksi konsentrasi gas berbahaya atau kondisi lingkungan yang tidak aman.
