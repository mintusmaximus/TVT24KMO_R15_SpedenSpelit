# Oulun ammattikorkeakoulun tietotekniikan sovellusprojekti

## Sovellusprojektin kuvaus

Tämä projekti on osa Oulun ammattikorkeakoulun tietotekniikan koulutusohjelman sovellusprojektia, joka oli 6 opintopisteen arvoinen. Projektissa tehtiin Speden Speleistä tuttu nopeustesti Arduinolla.

## Projektin tavoitteet

- Toteuttaa toimiva sovellus projektisuunnitelman mukaan
- Soveltaa opinnoissa opittuja taitoja käytännössä
- Työskennellä tehokkaasti tiimissä
- Oppia projektinhallintaa ja ohjelmistokehityksen parhaita käytäntöjä

## Tiimi

Projektitiimi koostuu seuraavista henkilöistä:

<table>
  <tbody>
    <tr>
      <td align="center" valign="top" width="20%"><a href="https://github.com/mintusmaximus"><img src="https://avatars.githubusercontent.com/u/156218528?v=4" width="100px;" alt="Jesse Heikkinen"/><br /><sub><b>Jesse Heikkinen</b></sub></a><br /><a href="https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/commits/main/?author=mintusmaximus" title="Display ja pelilogiikka">🖥️🕹️</a></td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/nnksknkngs"><img src="https://avatars.githubusercontent.com/u/169275720?v=4" width="100px;" alt="Anna Koskenkangas"/><br /><sub><b>Anna Koskenkangas</b></sub></a><br /><a href="https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/commits/main/?author=nnksknkngs" title="Pelilogiikka">🕹️</a></td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/eemil3"><img src="https://avatars.githubusercontent.com/u/181833755?v=4" width="100px;" alt="Eemil Koskelo"/><br /><sub><b>Eemil Koskelo</b></sub></a><br /><a href="https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/commits/main/?author=eemil3" title="Pelilogiikka">🕹️</a></td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/j3shi"><img src="https://avatars.githubusercontent.com/u/164160422?v=4" width="100px;" alt="Jesse Hirvonen"/><br /><sub><b>Jesse Hirvonen</b></sub></a><br /><a href="https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/commits/main/?author=j3shi" title="Ledit">💡</a></td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/jarmoit"><img src="https://avatars.githubusercontent.com/u/181835775?v=4" width="100px;" alt="Jarmo Marjakangas"/><br /><sub><b>Jarmo Marjakangas</b></sub></a><br /><a href="https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/commits/main/?author=Jarmoit" title="Buttons, pelilogiikka ja sound">🔛🕹️🔊</a></td>
    </tr>
  </tbody>
</table>

## Käyttöohjeet
- Rakenna oma laitteesi ohjeiden ja kuvien mukaan
- Kloonaa repositorio
- Avaa SpedenSpelit.ide tiedosto Arduino IDE:llä
- Lataa koodi Arduinolle
- Paina näppäintä `5` aloittaaksesi pelin

## Tarvittavat osat

- 1x Arduino Uno
- 2x 74HC595 Serial-to-parallel Shift register -piiriä
- 4x Lediä
- 2x 7-segmenttinäyttöä (common cathode)
- 7x ~500ohm vastusta
- 5x Pushbutton näppäintä
- 1x Piezosummeri (valinnainen)
- Hyppylankoja
- Vähintään 60 rivin verran kytkentäalustaa

## Kytkentäohjeet

1. **74HC595-piirien kytkentä sarjaan:**
    - Kytke ensimmäisen 74HC595-piirin `Q7'` (pin 9) toisen 74HC595-piirin `DS`-nastaan (pin 14).
    - **Molempien** 74HC595-piirien yhteiset ohjausnastat:
        - **DS** (pin 14, data input): Arduinon pinniin 8.
        - **OE'** (pin 13, output enable): Arduinon pinniin 9.
        - **STCP** (pin 12, storage register clock): Arduinon pinniin 10.
        - **SHCP** (pin 11, shift register clock): Arduinon pinniin 11.
        - **MR'** (pin 10, master reset): Arduinon pinniin 12.

2. **Segmenttien kytkentä (yksi 7-segmenttinäyttö / 74HC595-piiri):**
    - Kytke ensimmäisen 74HC595-piirin lähtönastat seuraavasti 7-segmenttinäytön nastoihin:
        - `Q0` -> Segmentti A
        - `Q1` -> Segmentti B
        - `Q2` -> Segmentti C
        - `Q3` -> Segmentti D
        - `Q4` -> Segmentti E
        - `Q5` -> Segmentti F
        - `Q6` -> Segmentti G
    - Kytke toisen 74HC595-piirin lähtönastat samalla tavoin toiseen 7-segmenttinäyttöön.
    - Jos luvut näkyvät väärinpäin näytöissä, display.cpp tiedostossa on kommentoitu showResult funktioon korjaus.

3. Kytke ledit 1-4 analogipinneihin A2-A5, muista vastukset.
4. Kytke näppäimet 1-5 digitaalipinneihin 2-6 ja toinen portti maahan.
5. Kytke piezosummeri (valinnainen) digitaalipinniin 7, muista vastus.

## Laitteen kuvat

- Peli käynnissä

![Laite Toiminnassa](https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/blob/main/img/Peli.jpg)

- Laitteen kytkentä

![Laitteen kytkentä](https://github.com/mintusmaximus/TVT24KMO_R15_SpedenSpelit/blob/main/img/Kytkent%C3%A4.png)