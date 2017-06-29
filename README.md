# Microprocessors-FinalProjectWithHCS12Board

Microprocessors dersi final projesi kapsamında Dragon 12 Light Board ile

otomatik şeker ayırma sistemi yapılmıştır.Projede 1 adet renk sensörü ve

2 adet servo motor kullanılmıştır.Proje başlatıldığı zaman LCD ekranda

önce beklemede yazısı çıkmaktadır.Projeye devam edebilmek için dip

switchler’den herhangi biri rising edge yani 1 konumuna getirildiği zaman

sistem çalışmaya başlamaktadır.

Dip switchler’den herhangi biri 1 konuma getirildikten sonra LCD ekranda

başlıyor mesajı ve 7 segment display’de 3-2- 1 yazarak projeye giriş

yapılır.Bu işlemlerden sonra renk sensörünün kalibrasyonu yapılır.Bunun

için renk sensörüne önce siyah daha sonra beyaz renkteki kağıtlar

okutularak renk sensörünün kalibrasyonu yapılmış olur.

Bu işlemler sonucunda sistemin üst kısmında bulunan huniden şekerler tek

tek bırakılır.Projede 2 adet servo motor kullanılmıştır.Üst kısımda bulunan

servonun toplamda yaptığı 3 hareket vardır.Bunlardan biri yukarıdan

bırakılan şekeri almak,daha sonra bu şekeri renk sensörünün altına

getirmek ve son olarak şekeri alttaki servoya bırakmaktır.Üst kısımdaki

servo şekeri renk sensörünün altına getirip renk okumayı bitirdikten sonra

alt kısımdaki servo okunan renge göre uygun hazneye doğru

yönlendirilir.Alt kısımda bulunan servo uygun hazneye doğru yönelince

üstteki servo şekeri aşağıya bırakır.Şekerler hazneye düştüğü sırada

verilen notalar ile buzzer öttürülür.Aynı şekilde şekerler hazneye düştüğü

sırada LCD ekranda o ana kadar düşen şekerlerin sayısı gösterilir.

Muhammet Tayyip ÇANKAYA