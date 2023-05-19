# Robot MONIBOT

Robot ini dirancang untuk memonitoring lokasi tambang dengan menggunakan sensor MQ-4, MQ-7, dan DHT22. Robot akan mengumpulkan data gas metana (CH4) dan karbon monoksida (CO) menggunakan sensor MQ-4 dan MQ-7, serta suhu dan kelembaban menggunakan sensor DHT22.

## Komponen Robot

1. Mikrokontroler: ESP32
2. Sensor MQ-4: Digunakan untuk mendeteksi konsentrasi gas metana (CH4)
3. Sensor MQ-7: Digunakan untuk mendeteksi konsentrasi karbon monoksida (CO)
4. Sensor DHT22: Digunakan untuk mengukur suhu dan kelembaban

## Cara Penggunaan

1. Pastikan mikrokontroler terhubung dengan sensor MQ-4, MQ-7, dan DHT22 secara tepat.
2. Sambungkan mikrokontroler ke sumber daya (misalnya baterai atau adaptor listrik).
3. Pastikan kode program untuk mikrokontroler sudah diunggah dengan benar.
4. Setelah robot dinyalakan, sensor-sensor akan mulai mengumpulkan data.
5. Data hasil monitoring dapat diakses melalui website monitoring MONIBOT
6. Pastikan untuk memantau hasil monitoring secara berkala dan mengambil tindakan jika terdeteksi konsentrasi gas berbahaya atau kondisi lingkungan yang tidak aman.

## Pengembangan Selanjutnya

Robot ini merupakan versi awal dan terdapat potensi untuk pengembangan selanjutnya, antara lain:

1. Integrasi dengan sistem pemantauan jarak jauh: Menghubungkan robot dengan jaringan atau platform online untuk memantau hasil monitoring secara real-time dari jarak jauh.
2. Penyempurnaan sensor: Menambahkan sensor tambahan, seperti sensor gas lainnya atau sensor partikulat, untuk mendapatkan pemantauan yang lebih komprehensif.
3. Analisis data lebih lanjut: Mengembangkan algoritma atau metode analisis data untuk mengidentifikasi pola atau tren yang berguna dalam pemantauan lingkungan tambang.
