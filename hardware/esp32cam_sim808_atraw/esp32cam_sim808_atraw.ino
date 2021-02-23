/**************************************************************
 *
 * This script tries to auto-detect the baud rate
 * and allows direct AT commands access
 *
 * TinyGSM Getting Started guide:
 *   https://tiny.cc/tinygsm-readme
 *
 **************************************************************/



#include "BluetoothSerial.h"

//Bluetooth Defined
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

String reading="{  \" : 1,  \"latitude\" : 12.983027,  \"longitude\" : 80.2594,  \"timestamp\" : \"%2F9j%2F4AAQSkZJRgABAQEAAAAAAAD%2F2wBDAAoHCAkIBgoJCAkLCwoMDxkQDw4ODx8WFxIZJCAmJiQgIyIoLToxKCs2KyIjMkQzNjs9QEFAJzBHTEY%2FSzo%2FQD7%2F2wBDAQsLCw8NDx0QEB0%2BKSMpPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj4%2BPj7%2FxAAfAAABBQEBAQEBAQAAAAAAAAAAAQIDBAUGBwgJCgv%2FxAC1EAACAQMDAgQDBQUEBAAAAX0BAgMABBEFEiExQQYTUWEHInEUMoGRoQgjQrHBFVLR8CQzYnKCCQoWFxgZGiUmJygpKjQ1Njc4OTpDREVGR0hJSlNUVVZXWFlaY2RlZmdoaWpzdHV2d3h5eoOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4eLj5OXm5%2Bjp6vHy8%2FT19vf4%2Bfr%2FxAAfAQADAQEBAQEBAQEBAAAAAAAAAQIDBAUGBwgJCgv%2FxAC1EQACAQIEBAMEBwUEBAABAncAAQIDEQQFITEGEkFRB2FxEyIygQgUQpGhscEJIzNS8BVictEKFiQ04SXxFxgZGiYnKCkqNTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqCg4SFhoeIiYqSk5SVlpeYmZqio6Slpqeoqaqys7S1tre4ubrCw8TFxsfIycrS09TV1tfY2dri4%2BTl5ufo6ery8%2FT19vf4%2Bfr%2FwAARCAHgAoADASEAAhEBAxEB%2F9oADAMBAAIRAxEAPwDklAxyR%2BPagr8nzOMEdRxilr1JuMBVztIbtytPw3CqvTrk9am0kFy%2FDGRCpXGevIptx5gfOFKYo3ZKEYfJz%2FdzkUtsgPYGqiVuaUaBAF7VJIoK1GxRmzWccC7o14%2FM0icDn1%2BtVF6hcsoPu9h1NPxxTEVXPXIpqN9AajQWxJww%2FwDrUo4A4607ASxxb3Arcs4FiQev0osrAXMUlMobSYo2ERtTaAuNpaYMZKvO6kpoVwxSFc9aQEJtoT1iT8qYbW3Uf6mP8qdwJ7ZVVwQn5Cqt8d0hNCWojLkX5%2BlJtrcjcCMZIqE%2BuKLdRNkWMnvUiD2%2FSpixXJETccLWrYQZJ%2F2VoYyyFp2Kg1A1Svod%2FwCNRcqG5seHrX7VNHG4HzNg11Pi%2BwS10dEtAE3Ntzisamup7NOoqSUe55a3h8%2Fx3QJ%2F3aaujwoP4mNbo8ipuWFsIVP3T%2BJqUwRf88k%2FKrvqZW1IyqjjaKj8vPWoAxwdk20%2BlFxLyAOpouDEhWi46r2qhEkIEsWPSnoh3c1fKOxJt9BUkn7qMIR855NS9gRVPpUTVIyPPtTc0CIyahbmgdxC3HFRZ9KXUaG7sCoi%2FrS8xWG54xVu2%2F1e401qBIfQCmbeORQkIbs64FNxQB0IAB2r94%2BtRy713AhcEdVas93cuwsI25bYnz%2BgxVmCDcQxHzGtE9SLGokXBqrc%2FwCt8sLjtuzU6ORdrD1iCpgfyqZIe5qtCVuWFX1FNcjjnFTdFFMq0vVjj0qFsA9PvVD8hbiQTbG2P0Hf1q0%2FdcUSGiq%2FfimfgKJMloem%2FaeMn2qYde%2Be1NaDNextlG0snzitQLVkodTaRYlNoQhrUzrTsAlFSgENQfcPtTGSdaKZIlNk6UgZc06RkDQIm4SHOdtY%2Bot%2FpMg7g4qkrMkz%2BaWt7kEDtUB%2B9zSfkAoGe1P2jFSRfUmjcA9K17GaJQ25u1TfSxsiQSx%2Boo8wHpU3KIRIGOaSZ%2BlBVPc3%2FCU6DU4lPTeDXWeNJAmkID1aTj8jXPL4T1H%2FABIHncjVARW8Tzam4xse9QMw3Yp3M2R9TTCaBGLqJEd7u7EVCDyfU1PQC3AmUFV7kHz%2FAGrQRJC2wjrVpZo2%2FwBZkH2ouxE%2FyKMjk%2BtVn%2BbnFD2KWxETgdKgaQZNK5RCZfaoDN17elLckhMppvmGhWATf61Hv9qW4xvX0H400jFP1GGMcmrMMoVcEUX10CxM1xGOKaswY4xTXmSPqJqTGdFtYL%2FD%2BBqEL5k341DBSXKW4kVzsU9uKv2yCMYxTtYEid%2FufLWYj75gFwHboveiCC5pxQNtG%2FFTtgDgU2wsQOf9qqkrNJNhSMdKj1KewvKrx1prRnH8OO1DuSmVig3%2FADY6%2FnVjeDg4zQO5CV%2B76incD%2Fnnn1709BihSRweDWnp1iAoyny4yvFUiUaqJirApAOpppDEpD1p2Ajao6QBzRimAp6VE65FAEQLJ1qTeKYBuFRPKoHNJCLH%2FCR3q2EdlbyeXGn8S9TWJNcbnY9WJzk01uBD%2FFzQ9dBCKzcnHFIFP0pXJkSBKYSCMruz71m2So6lR5GByO9MW7MfsKk1HfbZecO2M%2BtH26TuD%2F31Up9Sh6X0uOuPxp%2F9oSf3jmqnaxpDc09F1U294HZucjvXQeLPFAvxEInG1OeK4pJuR7VOUXRucx%2Famf8ACnHVEz8wx%2BNdi7Hhy3E%2FtJD1x%2BBpr3kf978KV7E9Rv2mPH3l596Z56nNPVjZn6ghmni6e9O24Bqo7WEWbQZXFWVFUQQNaxY%2B4BSG3jB%2B6hq42HYRqjLYFQ%2BxRAeagZRzSJdypNHux061H5fqeam5VxCq%2B1RsfWlcCPOaPpQOwYNL2pjA9KTb8tTqIPXNPjzu%2FGmSWWfC1CeafmUdRIuF%2BUFjntSwIOCyFT7VIrF%2BNGPJb6U2XK7j%2FOk3pcq10Pt5c%2FKwIOPSr9rbRn5gOtVHQRMbc1WuVxRIaZQmfjaDyaWNRHGTznrWYmA%2F1md3y%2BgNEhO75ap3HYqytjrn3xUSl1HAIH5gUovuTsSiT5OOfxpvJPH8VSUaVha78M%2BNvpW5FGIlCjoK26WRI8%2FeqRakoRqYTQITNLTAYabSAQim07AIWqIzClcRE9wuOaqSXSDO1uam9xlOa9YH7521Ta8Gc45o5WF7jBqfD8cN%2BlJBM0jDJzWlNc25MjQQcUyRFLhyPmX7p9M10RVjNsiC%2FP0qYLWb1DfUCOKqykZqb2YupRk%2B76YqHGGpamiDpS0uWxVgHTnFKKe6KJN1I8jE9ajkNedqJAfvetIf1qjLcZyB1pPMbrmhoXUN8gb5WIpvmt%2FCcUtQDzm%2B9zml%2B0y4wSapAOjvJFbjjNWhqjrncAT0ptokeNTyead%2FaCHtj8aRYG5Vu9NMq93p30AjeQVAZBQZ6kZYVGeakZE1QnnJNAwNPC420gY9hUQX3zTAVsldvagdMUPUY3P5VLGOOtIQ%2BkNGgzql5cZBwKliUycSbgR6Vml3G2XdwUVTkbf%2FALpNVy3AeOua0befYuM0%2BhN%2BhMZye9Vp27mpKKRjy%2FmA%2FhQTmjRIGMyew%2FDOKXzAoyOtLYTKzr%2BOP1pmPoaXkK4dG3cn8KnjgZkEmOBzj3rXQNi3aTXpiBgltPdSmKvLPqP8UUbN%2FsVNguH266QfvbKb8MVINYhBxLHLGfdDSUWO5Mmp2sn3ZfzqbzkPcUX1DcXIpymmISZ%2BlQ7xQMbNcxIvWqEl%2BqjO6mwKj6mvODiqcupHcOTUWApTXrszHrk1Ablz%2FFn2Ip8vQlkO7Lk4waaauKtoUJz2rQsxk9a0joRI006UyU81qzMVF64qbB75rPoHQhlz2rPn59alIE9SsUPrSbM9qRqKIiKPLz0qrAL5XFKIsVMkPcUxt3qJo2J4qUVcaUNRlDmmkTsxrA4ppTmgdxm05%2FGkOaBDPm2%2FrQxOOetO1hiZ5puR16VO4rBnmkoXYYbvrRvamwE3GlDUJiJNy96VnHajUEQP1zTOW7UuoEqLg0sn65oAOTSE8800IG7UduKEPUVI881JtoGJTce9JivbU61Ex97Ix3NX4IR5eTnP1pJsYkpK9Diq%2FfkVLbH0FUdzUyDjPNP1FYPNKr8%2FHtTFJlbLFttHoU9hh6kJ0FNPFH2hDPTcST7008%2FSol8WgNDCQvY9OgphUFqpASJHu4rUtbKKVd7hlfpuRsGqRIj6Chn822u5oZPVqSWa%2FseZ4%2FtKAcypT3Als9Yt5pNoLofcVrLcEr944%2BtIRXlt7S54lgjk%2FwB4VX%2Fs2BObd5bc%2FwDTN6L6DInS9i5F0kv%2B%2BDUbahcRj95bMR6rjFQDZWm1v%2B8kv49KrtrOf7wquUZVuNReTo%2BKotcF%2Bvf9aaSsPQZvbPLGmknd%2FFQiRM9Op%2FHrRz6n6VWggxQM49KEEhuKv2bY4Oavl6CeqNNDld1Ruct3rRmZPGP1qUj5algU7is9zzUdRpDe1KMcYrRGlh3anDFK6EO680Db78UtRiE0zAxSTHcZ8vvTDtyOKLiGHafekKrjg%2FhUbAMOMikIHrTAi2jHB4pCg9fpTGIUUUzZzxigEJ5XNNKelJIkRo%2BOtRlDS8hoTbwDSbW3Uhic%2BtLk0JdxCcu3pU6JgUyrijGRmpCRjFHMSRkY6VGRnpR5jFdffinwjoaOgElNPvQMSo%2FzoYHcR7GOyrbtgfLUgt7lJw%2B5mbaRmjd8vDHBrMYbv7ueKkH97sO9N%2BYEbfvTndhR%2BtLvwAi9KpAyKTAxwPxpuT1pSfu3CxGTz96mnO7FStAsB5J9qVFB9fxqrt6iLMEZ42dBWzBgRL6VaAsA04N%2FdJH0pAV57K0uDmaBGPXOKpNYy2r7rV5nXsok%2BYVNxbFX7U4G37bNn%2FprzTHvrqI%2FNKr%2B5GKLFXIH1Y%2Fxpt%2F4FVWbUiyj7wJ%2F2qSQinLdSSMdxzkfeqsWyxJyKuK1sLqIC2BuGPxzS4qn5Ejtw6UlJLm3GkLnmlxmmihwWn%2BWcYAH4mmnqSx4g9FJ%2BgqxDFirWgrF5c4xTcZeqZBajGBRJ0qSSjP%2BlU3T9aOpcRAp4xTwp5pPUbECU9YjzjNJe6xjxEe1L5fFaCEKHnio%2FKyagRBsJ7U0pRyssZs5ppBzRZkjNvNMKmgfQQg4pm360mMaVNJ8w6UxCfPnOaac9NxqbDEOaTmkMM03NOwCfhSHmpXmFhVGCM1MWGKewDG5wRSdaTEN3HvT4%2BTxTGTqvelPNIY003ucU7gJ9aZSBHfxRoDu602cjpuX8aGwKo54%2FlT9u7oDUNjuATkelOYF%2FkVue9P4hS02IzGEyoOfWjyyoA4%2Bm2mwuM3Y%2BbNQtljxUaXGIDls03O7aysACevrRpuIeEYjqc%2BtXIos%2Fe%2FWrQ%2Bhcj2xfMKf9thxuMgpCsPW5jYcOKeJB2IP40rgRyXIQcnH1qvJf9g3NFxmXdXiyrgncO2RxWS0g3HZ6%2FnTuBDuOfmbNM5PrRfQQdD7Uo61VxWA0lHOAppdhpp6ASLHxUqx8U0InSHp834VaSD2q%2Boi9LpcttHHJNGVV%2Fu%2B9RlETG4dfakNFtYBt4qI2zB%2B2KpSJsPxgVC9SZspTHuar%2F8AAcenNWpWNEOH61JS6kgeuP0p6dKeoXH49KQ5oUuhJC2cdKjJNKVtykyF2NR7iBTuWRljTd3pU8wrDSxI5phap2H5Dd570zdxQmCDfxTGajnAaW55%2FlSFqLjELUmaTYCZ96QkfWhSsIKaTTW47CrUoi3r1oBjWjI703nPNJjFZcgGpoEwtLWwEtJ6fWmgsMNJSAafemUbjPQmBSoDH5ozyKjYYzy9pxn86Qqd2OCaW4bk7IRF8oqeGEQQkv8AfNWiWrleTaNz9zVZ8PxQPYgfp0wPam%2Fw846d6mwnuJgMny%2FpTkT5aLDbJoxxxU%2BzjpRsK9i7BGHtjkVXaCL%2B4hp3aEVpreHYQyArWfOiR%2F6rCn1zTuNIjknZRhGIqCSdz82SD61CC5VMjZU0wmr3GHp%2FOj2oXYnqH%2B9Sj3p8xQbeetPCDPHfrQTuPWP06GphCM5q0iblmOA56fnVlLY89D9KNEMuRWwJ6VraYmnW86tqFlLcx91TFHM%2BgF7xDqMGq%2BStqkkEUagCNvasG4t2eJAp5BqYp2uOVriqJj8i%2FLtFXYIzj5%2FvVIitcptNU2WtEZsgltzjiq3kHJ%2FwoRRKkGTzT%2Fs544oJF8jPrUi29VcCZbf2pDbe1JIREbY7ahNvR5D9CBrfnmofswznvSiO43yCego%2Bx9qsLjPshI%2B7mkNg%2FwDdpPQYw2L%2FAN386jNmQOQRSYhptD3WmfZqkBptCaT7L7UO2xdxn2U0hgPpRYL3Gm3pPIPeluLmuMaA4phQmmUhORU8B4xQhse1RlAecU5EEoT5Kd2xUFhTTQMG46VHQA314ptID02aL1qtt28YqUPqRbfnpwjB5wKFowZZSMbd7%2FdqCaQt1qri3KMr56frVYv8v9KkYh%2FvfrnrTwgD8HgUmDSHYDHPXNOSLd070JiLIiwanUCmFizC3BqjcBASaBGfNIApxxWXcTN%2FhRAZUkk9Kj%2BU80yRcc8YxR%2FEVNJsL6CKc9iDS7f7wyKryC44LmpQn6Uxkiw55wcVKI801cCdYce1TpEKewiUbVHz96ljuLX%2BKRPxqWwL8VzAcbZFNa32dli5oQmU3hammFlGSK1IBM1atyPmBqJIpENxFuJqkYCDwKELlF8lsdKT7IzdVNMGami%2BH5NQvViHyqOWbHQVNqug%2FYtQeAPvAAOcYpt2JsURY%2FN0qwliP7tK4WLaaeSvypUMloFfaV6U2xpDZ7VFTIHNZj2pB5FK4yrNZsDuxxWXL975elSNFy2XMYNWQqhiuByKYWIobN4kADgipNn97FU2FiNohUDjHQHFICrIctgRkmmFW7pTQx20YppTIqbgV2Q%2B1RsjnrgU7sBuynqinsKkLWEdBmoJQNv8qWqQkiuVBHIFR48tvrTKJGxnjmpPLC8dabEHais2WIabTAaRxTfagBtNqNUSj1pIvOXj61QuIipIIxTVir6lerFpGCdzU7gMupdxIB%2BUe9ZlxKT64qOYZWJz2OD0pNmH9OaF5jHfwY24p6x%2FP6etaEXJo4ztDfMM9s1etrbKhiOf5UwBxhqjapQhvn7eBWbdXn7wrgn1xUsvoY80%2FwCAqozE%2B%2F1qo6k3G9vT2oYH0P0z0ougF59KUccU7IQuD7k%2FWphGT0FO9gRKqkjOKmSL%2FZNHUl2JkgHOVqdIhxxVO6K1sDfKc9vWq013s4APPfNS3cOg%2FUPltoePn3f0rM%2FefvHYnZ2FJbh5lyK0kW9t4p4trtIvX616JdOWkf8A3jWltBXKhkYelPH71aYisY2R8U278y20uW82nG8Jx603qBVttUSb5JPlkq8hBrHZjL9vEklXY7HB%2B5Vks6jQIvLhkO3GcVS1ONW19d4%2BVlAP60nuC%2BE5uKExt5TcsnymrscGaaA27HSme2D7gM1hX0BS5kU9jRLcSKLcVC7ZGBQUZl3u2kVhzRnOBUjLdl%2Fq9p6rzUgIlKtnDCruKxJyO%2BabkUMpET57VXfzQcYWpsJkex%2BpxmgjiqvoBE23FQNux8rfhR0uMi2sfvSN%2BBpTwepP1pX1FqN4p6gduKYMe23sB9aoXHzN%2Fs1G4o7lc5pduetXcZKsYQcUprO5SE96Z9KbH1EphpBYQmmYptiGmk%2BlAzvdIvprc%2BXdMAOivW1IfPj3cH3rNiRmt96nF%2F3ewU%2BhRUu5MHy0OKp55PvS6Ccho%2B9z9PaneWc0AS7A3UVZgh3DPWmtA2LUcGa0NPaKG8AdFk4xsNVcTZn3mwH5F2j0rNnl61IkzNlu9rfKw55rKnk3MR0qeUq5X%2BXkbRzQD1rTUQnenAelFguSBTT1WnYTJFQfjU8aZbqKVtQJ4ocbQf0qykOfu5x71XW4aXLEEKyyeVnBqIpJFcywyfKV%2FUdqJy1GBIZvKEHmv%2FCPWqV%2Fp11buvmwJ9VcGkught15l7qIjiiI%2BWrMGmt9nm8%2Ft9386pOwvI3LT7NqN88qkefFhiMVuMEbliaE9CepBsXnnNCZC0xhtZjzWjqsIHgSxi4El7MJPpwTQ9Igjzu6haC6aPpt4ytXrI3Ha4bI9T1rPzA9B0Wy0%2BTw39puZds4HzHd0NZen620ITzAJB3FO%2FvBbQ7%2FAEu5iu7JZIenp6VAzQtrGCyCVeee%2FFP7QW90wLezknvWaJdwZieDXQW%2BmIv%2Bs%2FKrJ3NBFCIEXoKxdU07MrSr%2FHUXLasjn5rR%2FwC6apBfKfkdeKZBXv7MLzGQVrnbn77cUkyyS2hb7P5ijIzg1KEVuSnNMBSuKiakxlV8HOHYGowq46tTEL93pURfrmgCrIYT0b5qjKJs7k0EiYppIo5rDImHo1PjbauGPShvQZC8zOSqsQn1qN2z2oQEYXNTKuOtDRCFoxxUGow02mA36000DGk%2BtMoAaaSlcZ2F1xHu37WHtRoOt4%2F0e63%2BgJpSXUl7mtdyKHJUg1Qab681HMUitn5uKXBJ6fj6VUWDfcmSJj2q1FbNT9QJ0tPm9qvxwKi8Yp9ADCj61RuPlbf1pXEUbu42DB4%2FWse4uT1yNwqWxKJlyzdB09Khzzn%2BtVrawDen%2BelGMnGKSkJEoTmnoPlzTGSbQU9amVG%2B6OOKtCLSJ69PQVNFb5Pp3qhFgxYfDCrunxectwg6oob9aXP0GQ6wv2KdniPz%2BbnGO1W9Qxd2Nvq1lCBNty6EZ%2F4DUAc9ORcYdN23H8dIu1Nm6WR%2FxxQw1NKzsfKvnO3Zxy1UdQvElIjX5gPyNTKXQa7kvhuRv%2BEkt8Z%2BbrjuMGu%2FEeRzVp6ES3IJIsdKasdCZNhxj%2BQ4q54umH2axVcJFBDn6USehSPOHuRc3DN27c1ZRyT8w4xSG9zTtTDHFuaNNx%2FixVq02lxTW4j0nwyMaQvuaxZZVm8QGdiNo3EH2waJP3g%2ByTeEU8vkEDjBFdXQyohRQUyF7WB%2Fvwxn%2FgNYmvafp6WuQUt5GYBcfWnzk8iMvXNOXToYTBOZd5xtPXp1rjLhkd8Iu5nOAB1JpJ8z0E1Y1I7N9JW4g1IfZ28vePmBz2piaXfMm82k%2BPXZWvLcm9ivNayR%2FfilH1Q1mz7%2B2PxpNcpd7kAVs5%2BWnFOKkRGRjpUEmc4ZKV9RkBkSP0WoJLmLPBouJIj%2B0LUTuhPT8BSbHYjMgHQfrUZJY9eKNRgDgU4c1SIJFGOtOpFIMU2kPYb3ppo2KG0w0mKwh4FR0AJTc88UmFjuyOCM1gX8RiaQDLll4qmuZhbuW4b7zgpyWDdParDoyvtYYNRbqNdiZYGzgc1cjsznmq0sDVy4lsKnT5Pu9aLhYTKx8mqN3fSbiqD5f71KK1DYoNdOhyGPHpUV1qKsvTmqexJkz3eQvbb3qhNIXz8xHtUWKIGbORnB7Ufez2Bq7CsOXPJb7xp%2BzIpaEdSdV4qSNO2OBQUidVqxHEc1S0JLUcXOcVes4g0oVvwp3HYsaraeStrcBsBiVx68ZpdFXy9QjbepSbMWPwzUibDxNZwovnSzbJtgGwj7xzWToOuDSXMciyNby%2Fe2n7tRzX0KtcZf3X2%2B8nljTh2JAJ5NYc7yC4KA42P%2FAHqY2Xxqt4ItpfkjG7vVDdnjNFg6Gj4cuBbeKbSTjbvKHPpg16p5QyQ1V0M%2BpVlX5qgKnPFKwER3DrWD4rv5BFFH5rMuBlc0mUjloZUEg9KvLN2HSjyFa5ZjlO2tOwY5FUhM9N0J%2FK8LLP6oXrikudiSkNg%2BUMfnSl8Q%2Bho6PdmOPPv1rqLfWUOBJTYi1c6nBDAXDZNc1e60ZG3E1JVzPk1yfy9omcL6ZrEubpJ5P3gDfWnYBLKYW8zeViJWGDt4rlFnaC9DxysGikyppLRjbN%2BPUL3Vrv7Rd3jtImNufau%2FtfGcIiAvIH3%2BsWOapuysh3uyLUfHGmrAy%2FYJ7j%2FYfaFNecxT7vmnt8xLyUgPNTGUuoSSNzTovDF5dAG9urJNoLJOBuJPatLUNM8KwnaPEHkt7neK2ddLoSoMwl0yLUNRmt9K1K1lVSBEXyvnVbh8F6vMpzAeDj%2FWpzRHkYndGUvhHVdQ3S20J8iNir5IHI61zL2iKuXP%2Fj1JpRJRVKxp91AKKkoXbTsfn9KpAxVhNShMUiLXFpD7UjUTHOe9NJNKwbjaQ0MZGfSm9qQhtMp3GNOe9M70gO58znjmqOqxJJEpZQdikD1qtUxp2KNuMQKuMEHOK2r6R%2FKR4ccnowpSJLNrdqf9cpBH8XatKGSOQja68%2B9QMlkcJVV5z2p2uBWd2aqkj%2B9VZEmfdzYOM%2B9Zk8uVbGM980Xuikiq8nXHSoj82D6VLYhQeMClC520JiSJQuFqZR6U7aXG0TKuamSPOKaAtwxk%2FwAP61cityOvFULzLiQ%2B1Lsw3H51NyjH1jUrmbGnTSCRbV%2Fkbv8AWoNL1Way1O2nkzMkTZK0lZah5HY%2BILzTtX0WPULdx8jmQBzg88VwMsouJzIv8VZJu4mi2A0Fn5%2FbB96yGUdSf0rTbUIkg4GBTcn8aL3AkibE24feByK9O8Oasuq6FH5xH22AYl%2F2veqb0Fy2LjEZ5qzaRwSyyCaTy1WPfuxSEZN3eF7Ub%2FupzXE%2BILj7Reuv92hblWM2FNqc1OrfNxT3ZJaibPWtixb7ooW5LPR2c23w%2Bt%2F7xto%2F1xXBTSFV6%2B1EviKvoWrW42WwwakS9IPWkIkudQLR4zWdPdfu6YFJ7nPeoUkLSdaAKl5cMCRvOPrVL5XYsT8x5oS1LE8xon%2BVip9RUq6jOMAybgKdlckc14zcmtGwnBsTkgcnvSa0ugW5iXku%2BY5FVcJuH7tfypIs3mutPWySEor8fdCVpWPjTUbHS1sLTEcSDanyj5abFsQf8Jdq0GkyWUV1%2FrsmSQr8%2BT1rl3JJ5JNDQX0GbeaXb271XUi5btNPnunAVGx612%2FhS3stGSa4udPW7mIxl8YUVblyk%2FEzm9cura7u2kt7SCDLZJhGBWXWe%2B5pZITpTOlFrDGmm0IYlMpANNMNSIYaj7c0yhpphoEdtEvBJNR3SGSI4VmPbFVe61IMmGTcp45z3HWrTT3P2byldAAPlXvWdk9CrlY3VzHuhaSQ%2B7GtDw6mNUgmzznH6GrsKUjp7wfPxVQ9KEBWll2LurMnn2nA6UDMueT5jxxVRnJahqwvMh%2FDrQFPBxS6XGTAGnrGPSl5CvbYlUfLxU6RnqMAelav4R2J1SrcNuWI9aewjXt7L1q41kxA2YrNsaJ47UhfmqrMmHap3GcXqLOdXn3jlWpi%2B9Owis2wyZxz64qRGK4aiKKfmX76682ARLHHHEH3fL9OlZQ%2BbG7gnil6EpWJpE4xUan%2BL%2BtFx2sS2685zmrlpdzabfrc2gO8Hae24elC8w3R6FLdjf8AcK%2Bx7UXuq29hpsrlS7yDaAKJbErU4O%2F1a7vJyfNkjj%2FhQHgVWlZ5MySHLtzz3ojEpvoK33eKatUSWlyta1m3IohqyD0zxi62fh6C3Xu6oPoBXm1zPuCrjvml1La0JfM2QqvpxUYnpiGvcc1BeS4NGhJTEuTyantGy0jenFKWwGddPukNRBs%2B1CuaFkRCSy8wn5s1Qk%2BRuuKu4h0bn1q3shlHO3dTvoTJWZBJbd94amfZ5T9xd30NLRD5h3kyKcMuKTY3tVE8w5YWlbCkV02keAdV1K0jud0aRSfMp9qIpdROR0On%2FDO2CeZql03B%2B6Fqe40bQrAeXZ2ys%2Fd2pTq9IiUW9znxeRu7NGoVO1Z%2Bp6gzx%2BQnA71nq0bbGK3Wkp2EMbrSUDGU2i2oxtM9qAGGozQA3NMJ4pN6ARmmZoA7X7VDBGNz4P0p0k37h2jx0605Ma8zlmP77sMY4zW1YyPJF5TghhQlbUmUyteriXPoK2NEjQW8M%2Bcl%2Bcf3aHdgzodTO2Y1lyvxQgMqe4H0NZlxcAsF3c0tiigzEngD1pvqM5qW7iuAC78479am25q1dIViRU9P51Lj1pkk6j%2FeA%2FQ1NEmaSQF%2BC3z0Oa2LK16cU7jNiO3x2qby8VDKGutZs6%2FO1SBxOs%2F8h276Y8yq7%2FKODiq2F1ICnINPQbk7%2FhTGBG4MoB%2BWokOG4qtxE7E54qFwQ3NRbUfkTQcc9sVYiUSNGH%2B6rCqS94mSOmeZufkqvrVxusYlb%2Fnp3%2BlaTWhCOeIAkY9s0x3LHOe9ZGlupNnMeM%2FdFJEPlLt0oQEyyHdzWxon7y%2Ft0DZPnRj9a0p%2FERI7b4j3eLq0tx0VS5%2FGvPnk%2FerWW8hsllciPOcVCsnaqQhyuPNqreS5nbrSEVw1WbZ8Wz%2B5pvYvoZztl6b0o8guXXfy9Ig5%2BYjNUFmcZwetAyWPkcgVevoorVf3S4oexO7Mp5C5%2BbmrGljfqUI9%2BaI6DZb1ufMxQGsfvwT%2BdJMZcs2OQo6yECvdnn%2BxWa2tvJs8pAnHtV%2FYMvtGZe6lJINu81z2pXJS1lbPaosao5fzNkexaqvknJp8o2R%2FWozTtqIZTaGAw%2B1MakMQ9ajNIQymE%2BtKwyNqjJpMOhGTUWaauB2MzRrw8RPvTwrNAQide1PXqRsZpifzim1lxxt%2BlTD5TzVapASOjP2OG9q0NPXyrKLbnJ%2BY59abZSNXUpwyKc571hXNxtODnPfFSn0EZc1x8vf3Bqo8mW%2F1mf8AZ9KloZH%2FABZyPTrTwOO2adrCeo%2BNeNualC4p7jHD161YEZ74BpiZbjTpjNXre2ZmU46UX1A1re2PU1r28PSpAvcAVG1Tcshk6VRlFAjhtVH%2FABPbvnnzTmqkm7BqlZgRK3ykHipRwnrT2FuRebskyD%2BtO2ky%2BlNAx0wDZHbNM6j3qQY%2BBcsSakZ%2BRtPy%2BlO4hszzW24JJJH9DXU%2BLFDWWmzDAWTLH34p3tuHU5g4ZGx27VDnDgVKAsR5znsOtEhO4AdMcUWAePT0rf8ACY3%2BJNMj7G4H8iacNyd9DR8e6h5%2Fie7QH%2FUhYq5YPulWo8yplp3z8nrUPmGJMZw7Dmq6CI4pP3mfxqK65l%2BXndTYxjbY1xzvNTRcw7Tn5hxRsiTO5LYp7fLxnmgZNfZW0TqMccVQWluBYVcTKD681Z1Wcy5PrzTZSMwGr%2Bk%2FLdM7fwpwaGDK13L5kxPNQihIDV8OL5niCyjPTfuP4CvS5LppMsx5NPoR1KskmaxdduNlqqf3mqWUjCzmkPWrGxlRtxSAZTTSERmmMaBkZplAyMmoy1JoRGxqNjgUmgIiaYarWIXPS2soB0iQVH5IjHyU7824uUrwNDHqFx9pj84HoKstqGnQ7ttgy%2F7pHNKd0vdDlTKzaipTdBDt%2FwB41UivZXlcMQP7oWov3G9CW9vP9FTcTu71iSXGc8k0cw%2BhVdj16YP9%2Bm9cZHNXLUkl2%2Fe9e1P4600uor9SVE9f0qUd9tTJ3Y3qTYL8tzkd6sRox7cmqiHumjbQZb6VqwKsQ3NiiQWL0F7aDhnxWlDdWjDiaOs7jsOaaFm%2BV1P0NRM9IohZzUbrxVdCTgNX%2FwCQ9fZ6%2BcRVGT0NTuNjVX5dxzTv4K0iSRN1qReHANIYr%2FjimZzj2oS0AmRtsXfmo35pCQPzBs%2FWur8QHf4U0Fx0PX%2Fvmr6D6nMBtrcUfelxUiHSEKm1fxNOR90eOcr70xWG7ua6TwLz4wsT2h3SH6bTRGXvAyjqE8%2Bo3V7PgbZ7h5g59PSqEDZcGosD1ZPI%2Bw7j17VBK3Rh%2FF1q0MRGwrHPahchfm69hQKxWzkk1KX2IvtQ2MSTCEuByelVhywoAsmQStLEfunkVWT9zlm%2B8w6Uk7CQ6OQiUN360l4d4RhSKGRBUXfKD7VPZvsUsetUxFGUCOVlyWx0NHbNDGb%2FAIMQtqU8x%2F5ZxYH4mu03cVWxBE7Vzmuy5vtn9wYqS0Vk%2B5SfWgBh60xjzTtcCM1GzDtSQxnaoiaBIYxqM9aBEZOBUTtQMiZuKYfegZHmmZouI9XqOWnYZz5lzJMwdl5%2BXBqC6lycgY9qgWxCkjCJR5jZx1Gc1BJK3mbj93HPzdaVuoEfmAR%2FfytRZJo31ZN7ibNw7H26mnqvrtzV9rlPUlRfzqRUyQOM1TAmCZPXP4dKmjXrUC8ixFF7VpW9t7cVp0A0lgEcRKjFV%2Fszu2c5%2BtRa4XF%2ByTDouaQ27%2FxRUcgrgFZTkR4x7Vctb%2BTeEl%2BZeme9LlKNYCkZfloYjz3Vv%2BQ3f57TtVTb5ihjgEdaS01KZAX3Sf8A16kA%2BXbTtcBm3A3NQnMmenOaemwh7oV6dDTFXg0uYRKi7k9xULHJoS1KJGARF9a6LUB%2FxQuj85EXGfwpmfU5v%2BL1pw4kOOtQ9yxHp0Jwaq4mLGhaVUX%2BIgV2PgVYk8R67dqF8u1sX2fnT3EcraGT%2By7cb35bpn%2FZ5pbfqTUWtuMbO2WpobcmPTpVNdgY6HAj3HpUHmsZtzc00u4gkXaxHqae%2FwAqhm%2FCp3GREmZCe9RAfMKYyQbUJds%2B1Rz8vmktxDQTvNSIPkO48elPqBDIxLGk34Si4CHDjefSo2bNLyGdd4STy9PZ%2FwDnpIfy4rod1WQRMa5K7l%2B0Xssg%2FjfNItFlB8oz1oPFPcRC1RtS2GRGomo6lMjJphNDuSRsaiJoGQs1R%2FeNK4CVE5pisMpKLgemQXAePPfoabLPxxyaJbFHLNKfOfj3xTZ5NyjHNJC5StuA2jjbig4LHd%2BFIlDO49KftyVOTx6Z5prUbj1HrGp%2B9g46ZqXb6Ch9g6WJQvcDmnom7OR%2F9aqsSydV54NW44v4aLiiaVrB6ZrWtrek9xl%2F7NvjINVo4wDii4EwjpRHTARk4qlNEWbjk0gNRB8vPWn0nsM831QbtUumPT7Qf1qsxCELx71EWPqRMmyRlNOHNWKw24BIBU8UkPFLUokALfKVqvnc%2FtSegvMsIwj2k06RPKBI6dhVXAh%2B8c10uPN%2BHPHJhkwPpkUbIejOb460itht7Ek0xCzpslPTB71ImETLDk9KXQYitiVW9Dmus8L%2FALjwt4mvh%2FzySHNK9iepzkUqrZCMMcglgM57CmQ%2F6s4pjuQM26gbttFgJGf90BVahPURJ1xTJGy%2FWjUYiHB4oAAcntQnYRFI%2B5qdnMY56UeYxIhwS3SjzPm4zj0oEMk600rk%2BwpXYCg7sgfhUL5x71SYzvtFg%2Bz6dbx%2BiCr5qiUytfSeXYzMDziuUbvgfSpuV0NMU16LiIGPNRNQVbqQseajY0DImPFQk0ARO1QM1AEZbJp3SkkJjSeKh70xCGkoC516XJgkfAzk0x7oqu38vakEdTNmlyzc8e1RfeHGTSjKxVrDXHHIzj2pwAHPpVc3M7GY5V5qTBxntQ1ZjJACeoqRUGe%2BPal00GTKvvj2qVEPHv3q7dwLsMX3Qe1aVvbZxmjSwjWtrYADitKKHFSwLBT5TispFIcg9RQmBZAp%2FSmgGuc1SfIfNIRoL93nrTu9JjPNNRP%2FABMronOPOaqEuT1ojsVccjEx%2Fd5WnvwuBVN2EhiHgqenahPvc0ogPaXa4I6jpTH%2B%2FlO9SBPCoK%2FvOB7UD99G46EU0IhArpbM%2FwDFvr8ek%2Bf1jpy2BbnNSj5sZ%2BlMwe3NJFF028tuMXMXzL0qqzFnbPHPT0pXJEPtXW2sj2nwouv3E4%2FtC5XbJ%2FCR8tU1zQYJ%2B8cqOhqSIccml0GNaP8Ae7aRj%2FdqbiBxuiyO1RIuWqxvYcPv%2B1RMPnpAKB1pOqUmgIu3Wl520W0ATouKQCn0Bg1Nds0CGrToY%2FOuYk9WFOIdD0OLpkVJQJGTrkmIFT1OTWGvNxGvvQl1KNWo2p3Aheq7UNjImqB6m4EDmoWPNMZCWqJjzUiGDrUrfWmIjNRd6LDA0lN6jsbzyHcevTH3ahJ69QR3FLRIQzr2x7dKCoKnPINLYJdh3Pct9M9Kk570lrITHIvAGalX%2Fd%2FE8VfIxXJAOfb1qZBj5scUICdI%2FXFWooc4wMCmtxmlbQdOK2ILfFDJNGGGrSrSAeV%2BU1jc%2FaG3%2Ff70kgJxT6YDH9qqOdrZ9KYzRXlc0h9aliOB1XSb2P7VeNEPL3l%2BvQZqjZaVqOof6iA4%2FvNwtVGAOxuW3g6bZm6v40b0RM1h6zaf2dqc1oHL7D9%2FHWlK3QImaWxzmpTuB2kMpPqKOV2KEk60qPxio8hFgfdHpTB96miriEKZR2Wtuymz4N1VRnhv%2FiaLaCMMHdHyelQ5%2Bf5vu00BfMp8tjuJOc4Aql%2FEcVPW4Fi1RWjunYE%2BVGGH%2FfVdZqshi%2BEnh6Isf38uVGewzVy%2BEFuceetSL9z5aEApJC%2FL972qAcHFKwEgPzbccGmOdvyr%2BNHkJsZS4LsDmhDEf0psfp60wI1Vs4PWiTHQdKQA3KA0xBnpSAU%2FeqJ%2FvUwFzV7Q08zV4yeVjG400tQex2y8cU40EmDrcmbnyvQDNZ9t814MjpR0LNQ9KjNHQCGSq70gIGNQMaEBC%2FWq5pgRE1EetIBKU0IBtNpgJRUNXA1i%2FwAvH603PzcHp2xT2ZNh2Oc0o46mqumIkAYcAZx2zTyp%2FizVLQZJgDqfxqZUbv0pNkkiLz%2BNTxxlTxz79Kdh2LsUOc9Dk5z61fgh6UAaFtD%2B8rZii4qWBaRcVKKQD8VlDa93MXB3NTjuDJVt07SYb0IqP5t2GGKpiGuPSqj8HmpGaQwenSkYcUDKDRh4yrAEelWkGUHpVDA1z%2Bs6Jb39%2BbmR5A7DBweKBM5a80ie0vFSMNNGJMBsV3dzapcb4L6ITJn%2BPmntqSYmpeFbPyt2nyGCQZ%2BVuVrjY%2BdtQ3qUXP4PSovpSKE3ZPatGwO7SNThHXYGxTbJ6mMTnGaDzxmi5Qi0oOOKCWyeKQpHLt485dh%2Bma6%2FxL%2B88M%2BGbKLe32LTxMwA7Nhc1r9knm1OfazWL%2FWHc2O3SoA3zbQvHbiotzbD5rj4JDa32SWGDVeRxLdSMqbAzkgelSPoOf8Ad%2BhNRv8A3qHqIZjdUgxyg60mOxAeBimjOadiiR%2BBUDVBPUFOeKX7o2iqWwyNulDfNg0AISBxW54WizLPKcY%2B6KcRPY6kUjHimI5e7l824eT1pLD%2FAF5pIZpGojTCxAxqtJU9SkV3PNV2NMZCTULGgnqRNUdAxtOzgUmIYaSgYd6O9IRtDcCnXApiq2z1PfNH2tQSQ4ISM%2BnQ1Iq7jVaIOg8DaxVf%2FwBdS7egehb6DRNEuKmVST82apImyJ44s49zmrsUHsKfUReii5HFW446AZehX98K1kFQxkwFSYpCHAVjTv8A8TJ5F9aa3AnkuEKHYgBqujMT8xzTEK4x6VWfGeeaRRqhFx8n3e1NZPlP0oEZNrJ51kkv95c1dB%2BQUyhO1VbocKaYzLUf8TRFHdq6EordsUWJIjbRt97kVjz%2BDtIfcYxNDn%2B6wxQFjnNe0%2BPTLiKJJi5cZ5rG%2FiqLjRJtq7pv3L9e32elETMiMDYMccU7bz93HvVL4ig2%2BlS21ubm6SHueapImRuNYTSrsmhJXsSRxXe%2BHoP7e0a8tNQmSB5YobNRCfm2RjrV8vMrEpnFNbpbySW24TiFzHv%2FAL%2BO9SxbV%2B6gX6Urgjmr5917Of8AbNRxff8AesDQU8tzTf4MUc1hC9ExTO%2FFU2MJuW4FNPyjimmA3qmKi70hD844pJB82aBhHC02cY465pvTgc0MkYa63w7F5enKSMF%2Fmpw2EzYqC6bZayP6Ci4zlpOlT6d%2FrG%2BlA2X3qJsd6YyFzzVZ6QFZ6rvSEQsahahDI%2B1NoAYaSi4rjaWmULRSuI2tvJ96eq87SOaYhyAZ4qQR9DR6iJfwqUKev8VMdiYJ%2FdDbc9SatRRU0rIXmXYoPTirsUFVcXQtpFip1SkBYjH71a00FSBKBUgqQHiqF1p7GVpIhndztpoCi8Mkf30IqLGaYCBR%2FDn8KmjsbiZvlQhf7xoC5sLAI41ReijFRyr%2B7b6GoYI5rw%2F%2B90eLPpir8ed209qsokkOBVW4%2FwBUDQBn2g3a2nsf6VvUxD0FNego53VtKt7%2B9M027djbwa5bXdL%2FALOeKSPcbeQ7VLHnNTpaxPUox%2FMT7Vd011jnl8x9mU4NINylL80skjn53bcaaEzQUOCnNXNETzNQebsi4FOG5D2OiB4q%2FpOpTaXNJdW2zzPKK%2FOKc9ggctazNMqTScu3zGtFT8hOe1O3uj6nLS%2FOd3rzTo6hDF6UylygKaBRYBrffqM9aSQC9qi71QCrSt0oAZuIHB60i9aQCNy31rvbSLyoRH%2Fd4q1sQT1Q1mTbYbB%2FG3rSLRzkh6KauacOXouDLpqBzQMruaryUAVmqu1AiFqjNIZG1NNMBh6UzNJAJTqBi0UCOi25PPT6UuzYCF%2FQ4p21sSS7DtPypx7VIsYGSB14prcViaKDnH5561aW3wcU18QFqK3OflH51ejtsGqAtxwVZWPFICYJUgSmPYlCfMK0FFQySUCpFpDJQKfikBFKdrelMX5nGTmpAsM0gHycmq1ksgDrKuDnNMRYK1VuhiGT%2FdNSxnI%2BF%2F8AkGqvof6CtS4BV81qiiI%2FMM1HP%2Fx6596GBW03%2FkJyE%2F8APOtmgRKvSq8poKM9%2B5rmfGUn7vT4M%2FNvaT%2BlIRzv3D%2F9bFJu5pB0El%2B5UKM6%2FLQMmkZwOm41vaVH5SbD1C5NXAiRo1LBEZEdj90KaJFROf06PFpH9K0J%2FwB1Zyt%2Fs0X0F1OZxUq9KzGJQetNgJThSERnrTKewxzcJUNADlHNI9AEXanjigCbT4%2FO1OEf3Wya7lOlPoTbUWsTXJN1yi9lH61JRkdXq9YjapqvIC0zVA555ouBWeq7tzQMrNUBoERNzUXQUrgNNR07lDajNAAKdQIfQaT2JOqWPPc%2BmKRE781cVfQIlhY8r0HsasLFnI%2F9BpWGWY4Se2Kux2xx0qgZcig6VoG3weKRJMsVP2VQChacBQBKBytXlFZsRKBTwtK4yQVIBSAZMmVp1lFEoJkz6AUAOx83FO20AMYVUvR%2Fo0v%2B4al7AjifCLgxypk5VFat%2B6XzLdsfeHStSiiDiAZpk3%2FIPf60MYaOmZ7hz%2FcA%2FWtPHpVCJE6VVufumgCifu1xviuUPrvlf88Iwn41nIaMFuHwoxRnmkIkzkD60v1qvICe2j82RUNb1ly85%2F2sVpEkur1q6522r9gFNTIpHM6UM2NuefuCrOp8afKPb%2BtHQXU5zq1Tj%2FV1LKGnrSd6lAGOaXpQIioFAxsxwtM7c1QDlpj9akQzHNSU9xGl4aj33kkvovFdWOlMQVzN7KZruRuuTS6lFfhevWrVu%2FyU3oJErGoXbijUoryN6VXc1IEDVC1NARVE1FxDKaaEhjaioQxw6U4UXAWlo8wO0VOMbvwAqWOHkd8etO8tjK1i1HbZq5Ha%2B1WhlqC259qvpBxSEPWOtHy%2FlBFMYBaXZVCG7aUCmA7FX0HFZsCYLTsVAyUCngUDHEcVHsft0pE3JEWnYoGNIqjqH%2FHlP%2FuGpYzz7wq%2B2%2FlT%2B9bLj866xelarYOpl3vyTFfxpsp%2F4lHuaBlrSUxZ7j1ZquVTESY4qnd9qQyrtzIq153f3H2zVru5HKyzMV%2BlS9xkUn3B1qi33s0Mklj609utS9yjT0tBmRj2GK0rHlZD6vmtI7GZoRD5qmnOy0kb0FEi4mXDjd8owKraw%2BLXHdmAokvdJtqYIqxgbKz6aD3I8c0ChgJ3px6GmMh7cUoosMZL1GKb3piH9qhejzEMHNSdFoHsdB4ah22pk%2Fvmt48ChklS7m2Wkzei1z9qV8znpSsX9khl%2B83OeaWKXaKbETeb71Gz0XAhY81G1ILkB61E9IZEaZTAYaaaLjsNNR9aAHCnCmhA%2BMUgpMD0SOA4q1FBg1fmToaVvadK0I7Wm2SSeTg1agjBiNIRDtq7Gv8Ao8f0qihcUYpiGlaYzInU0AWYLWedN4hcL6txV5YsCs2A%2FbRipKH08CgRKBTqkBMUhFADaztV%2FwCQfc%2F9c6TGjzfw0%2F8AxOLdf70X9K7hQAnNbR2D7Rnamg%2BWT8KS4i26Rar7CmMmskxaqc%2FhU602BIOlUbn%2FAFtSBQvp%2Fs1pcXB%2F5ZxmvOLZcRIG64rO%2BpROVLR1XMG6mIXbsOBU1vaTXs%2FlW0fmSY3bc4p2ZBrwWstqjJOhR89Kn03%2FAFRrQDTjqHVrgJZGNT8zVmy0VbYHZzVHVoJ5o1MaMyIcnFUyDHUc1O%2Fas76lDKTFDAKH6cUARUoqhkbjmmikBIarueaXUBRSS8CnsI7TS4fIsoo%2B4WprssIHKHB205biiczJdXbRFZZ3kU%2Bpp0MZXfuHzChFFYjmos4NDEO3e9JuotYY0moiaBEbVGaQEZplMYw0lIA2E0oix1p7ALjFFAELUlK%2FQD1qODmrSwVoQX7cqBhqvxNEf4qQCz7NmVIqSzwbYCgAMSetSwJ%2B5%2BlMbEfagyaWOO4uP%2BPW2kf3PAqiGXItFY83l1tP9yGrcUdlYtm2t1V%2F755NZyl2KjG5XvnuZj8sp2%2BlV0%2B1AcbyKysabE8UjscN%2FKrG2mZiipAKYEgFLigB2KYaQDGrM1f%2FAJBl1%2F1zpPYZ5noRC6zZt%2BH6V3U5xxW3QClexvcIgHC0t9%2Fx7xLTsMktuLCP6VJHzSAmWqM3MhNJgc74rm8rSPL73EmPy5rj171PUZP%2FAAUwUCIj9%2BtPwy23Xc9vKalcRram24S49ciq2m%2F6knturSIM0DL5a9M1iXJMyszc84FHUroaMQ%2BWrF7KbTQZnHVvlH40VCYnJRDGB2p5%2B9WQxP4aaOlUCFpr0DI6UD8KAIBzzzTu9GohHOKhNT1GOWpIE827jj9Wq%2BpJ3KegqDUDts5j6LUsSObQ7nhUduTUkMu25w3KtwauG5VjUbTrc5wMf7tVJNKiJOGcH61RJVl0uVOUkRhVU20o%2FhqGHMRNE%2F8AdNRFGz0osMZ5cn901GY3I%2B6aLW1GMMb%2BlN8pjRa4x32c0oj20LsIMcU08UWER001IyA%2FeNLSKue%2BQafFLcRxfNmRsZFT3WjGD7kwP1FdMjHqQwadPNJ5cK7mpLm1lsmxcxsmfbikMSPEnCnNX7bT5pjiJDj%2B92o5QZfTw%2FNj5p1H4VbttHMQ2vNlfYUc0egWZbi0yzik8wQKZP7zcmrMq70wDis2%2BYrlMmTKNt3UzC44NSy7kSXkUNyPMj3J0z6VuLIjKCjcUGbKc3zTUzFIQbaWgZItPoELTCKQEb1ka7xo17%2F1ypFI8vsm8vUbY5%2FjWvQb0fO31NbdAK9v%2B8So7wZAoGSwLmx2d6bZndEDSEW8hRzWdj5fqc0FHH%2BLpN1%2FFb%2Fwwpn8TWCnAqEwZO33aYopkkOPmauh8M%2F8ec7H%2B%2Fj9KaALjItyG%2BlO03H2b8a0iSLey7UKD7x7VWljCwRqw%2B%2B9KxUjQjX5QKqeJZNlta2oPfcamoETDi%2B9RWYxp6UHtVoBBTW%2B%2FUsYylbpR5iIKcoqkwGuMGoqAHYrQ0CHzdT3f88hupL4hM65elVrxPMj29jQxo54lfNkIwB2qv1NMZ00B%2Fcrn0pHFUQQt0qrLTEVJB6VXbrUDQykNUFhKYRQUR8VG1SIiOajI4oYxhqJqYIip1QM%2BhNOmz4jgEj8AkfpW9qK%2FMa2ZmiDQ1%2F06T2Wt10WRCrqGU9jUyKRUi0nT4ZN8VnCp%2F3au0nJsdgopFBRQIzL5Bv%2FAArOkfHFUQUJqtaTI0cu0HCf3aTEzVz89OqACnUDHAU8UCHU00gIZKxPERxoV8f%2BmX9RUlI8zCDI%2FwBkiu2tbsX9olyn8Y59jWqYdSe2iCLx1qO5xuqiiSzOUNOii8tnx90nNBI2XJ6VXmURIS33VGTSb0KPONSuftd%2FNOePMOarxio6B1JZPuVHH93inzBYiz1ro%2FDrf8SWb%2Fruf5CnHYTQXxxAadYr%2Fo61ZPUjmT%2FiYbnGcAYpZfnlg9A1AMup94Vg6%2FN5uryKM%2Fuv3f5VEtxopp0zSdagYHpTG607ggH3aj7mldAJRJ0oCxBUgqgIpDzTcVIDjwMVu%2BGIv3Usx6k4qxM6AVgeII7sXIkWSTyHGMKehqGlfUaMtRiPk5zSqMGqQ2dFHwgFSHkVZBBIPSqsynFJjsU3BzURpAyMim0JgMJFRE0XAjJNM5oGIQ1M2GjyAaVqCUYFAyKlApJ6Ae8Xa%2BXeJL15rr7fZcWZeQ%2FjWxkM0uLZdy%2F7tatRMuIUVJYUUAFFAFLUU%2Fd76xCjPJ6CriZsiuUAfFMts%2Fao1XnJpCOklizytUopw9Z9Rk4p4oAcKkFIAxTDQBBJWF4kONAvf90fzFSNbnm4yTnmtCw1Y6Vn5C8LfeHpVbAdNaXkN0okt3BU9u9LK3ztmtLjFg%2BVTU6cKBTAfWF4ruPL0%2FyVPzzfyqZlHAzAtJjuelSLGFZFzz39qRIyU02LocVAXICDtz2roPDzf8Sidf8Aptn9BWkAY%2FVP%2BPWrtumyBF9BitehI2YLvBqPg3AA7DNSBbjZY90jfwqTXGh2lYyP95%2FmP1rOT94smGVjptFgYUxqAAdKjNTuMQdabKecUxMZ3p4oEQNzQtIAk7V1uiQ%2BVpcC4wduTVIRp1n62f8AQlX%2FAGqllI5%2B4xu%2BU01TmqSA6HHy0zNaECScjiqUjFakZXdjiqrMaQDCTTaAGEU2gBpIpuaNihpeo%2FMqbghN1Qy9Kq4EApaVhHvt8vzcCrmmXTmHa2R61qZmnaXZS4%2BVNwbitypkWgoqSwooAKKAGyLuQisGZsNiqRnIpzq8mMUtt5tvJ5i43e9QwLy6hcY%2BZUP0FV4pcS9Kz6lGgpyKlFWSOp4pAPqI9aGIikrn%2FFP%2FACL17%2Fuj%2FwBCFJlI87XhCx%2FCqpfcTGx%2B9WgiS1le3uVaBtjeorrra%2Fg1O2BMyx3affDfx0hlu3dT5gkZECrnO6q76%2FpsI%2BefgcZxRz9i%2BUpXXi%2ByiO2G2nmPXPAWud1PUpdRmM82FO75Av8ACKm9w2Kk%2FwC53S7Pmf8A8dqpGCeetV5k%2BY%2BVWmjVxknoab8sS%2BXnLD7xoFuRjBBT1rR8PsQLmJgeu7pTpgy7qYzHbj1krRIxxWgiC5%2Fh%2BtMtfmmlPYcCkBYu4wmmXczSDAj%2B7XHRKfLX6VnLVlWZOf8AVimd6QCetNP3qdxi9qiakIFqOT%2FWGi4wpx6U7gVTUi0BYAhluIkHdhXeRqF4UcdqfQnqOrG1p8zhP7oqSjHbrTTVAdDA%2FmQq%2FqKY%2FwB6qJDNVLjrSApuKhIov0AYSKYTQMYaZ%2BNJsQymkUalEbCo8UCG1E5o3KEooJPp0rEkXESq3tWFeS%2BTMTVX1FY09C%2FfThj25rpKcwQUVBYUUAFFAFe9fZb8d652Sb95TRDFjfL1fTpSZIuAetN8mP8Au1IydAMU%2BgBQacDSAfmmGgRG9cz4wXOhTNvcbMZAPB5FSykcAW%2Fc1msxd%2Fl6%2BoqtQLsDhX5645rPuSjn51B9MimA9XMkYjY8DvT76XJwOBSKIY28yLb3WrcKARb3APPenfQgqh9%2B6N%2F4qWKPGVAwaNgJPkibZ13e%2FSqrLsJ%2FnSuWRxozvkfw9619GO%2Fz2EaoN2OO5q4shl%2B8XJtB287%2BlWnPetAIZfu5NJZj7x9WpdQDxAdthFaofmlO6U%2BiiubyM4QbRWZXQJ%2BgPrTI1O3eelKwWF4IbFQmlcYrqahbrirEKp4qI8mlcBVpzGkBX%2BtPU8UeYy5okXm6up%2F55jNditV0JA1zF026V39TUloq0lUmI27L%2Fj1QVI4zTIGdqrzUAUmqtIaCiKm0hCUw0bDG0bqAGbqbxQ2BC1Q0ih1GKaEfRUjXEq8nyqwr2WEOwEods%2BuaOojV0WVoEjbdtFdOuoQ5AbINWxIsrIj%2FAHTmnZB71FmULRQAUUDKuo4%2ByEntXNtkUyJD7f5nzWgtJkj6WkA8U%2BkBFJvC%2FJ1p0bMV%2BbrQMnFNNICJjXN%2BMT%2FxTtz%2BH8xSYzgRJthHfLVRdRbqDwZOgNVcRYtE3FDnqKqS2%2B%2BfYOeM5pFGja6BqN7apLZ26eQeA8j431JP4S1WRjmWyj%2F3pKvT7RlzkaeENURPMM9myj%2B6c1VJP2eMccmk%2BXoVEy9%2FIIq9Dks7Pw1JlFSZ%2FwB5UgJlRAxyRxQMJpPLAjiyVHv1rTsNkErmObzYp3%2Fu421URM0Jcb4f97%2BlOJyaoAmH7gn2qSwi%2FwBGi%2F3aTAxNUnW6uJ%2FK52fKPpms6BSWrNFMlUb88HFRyvlioXaopsXUZnbg08oImJbafQUB5EDfMaZIM4alcA27OvWoaYxE5pW9KVhFc%2Ffp46U2M3vC0XE83947a6OqZJFdvstZG9BXLzH71QWQ%2B9MPWqEbWmn%2FAEX8asGqvcgiaq8tDQ7FGQ1A1GwEeab3pDGHrTTxQwGU2gYymUhETmmimMdSGptqB67NeXd43lySu4%2FuDpTEhKN0x7VS0JbNOyjcniR0%2FwB2uij89xnzFZv7zjmgRoQOVXEoJ%2F3alzHHk%2FvOfaruMry6uifLDDNn3xirC3czR5VD9cVI9h6XjCMGQf8AjuKX7fk8Rf8Aj1NxFzFS%2BvGlgdNoANYpbmlYG7lu2WrdSSOFPFIB1PpDFopiHCikBG9cn4481dFIj2bGb589aRSODTiPJ5warTgyp%2FtCrYE9ox5C%2FwB38qvaRb2tzfiyubjysru3f0qRehqzW2kQzWkcmsoHsPlGI2ao9TtvD%2BrXRnvdU3uP7tuRRz9bCSlYtW95oWneHvK0%2B8aRYs%2FIy4LE1y6bVsoYz1ZetT8Wo9jPWLyIxNLkOei1ZgG7czdxmqYdSjJG3neXxuHFTiNE\",  }";

BluetoothSerial SerialBT;

void setup() {
  // Set console baud rate
  SerialBT.begin("ECOV-DEV"); //Bluetooth device name
  Serial.begin(9600);
  Serial.println("AT+BTPOWER=0");
  delay(1000);
  
  checkSignal();
  connectGPRS();
  postData();
  delay(1000);
}

void loop() {
//    checkSignal();
//    connectGPRS();
//    postData();
//    delay(1000);
}

void connectGPRS() {
  Serial.println("AT+CGATT?");
  delay(1000);
  ShowSerialData();
  Serial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(1000);
  ShowSerialData();
  Serial.println("AT+SAPBR=3,1,\"APN\",\"telkomsel\"");
  //Using Indian Standard Vodafone Sim and so APN is www
  delay(1000);
  ShowSerialData();

  Serial.println("AT+SAPBR=1,1");
  delay(1000);
  ShowSerialData();

  Serial.println("AT+SAPBR=2,1");
  delay(1000);
  ShowSerialData();
}

void checkSignal() {
  Serial.println("AT+CSQ");
  delay(1000);
  ShowSerialData();
}

void ShowSerialData() {
  while(Serial.available()!=0)
    SerialBT.write(Serial.read());
}

void postData() {
  Serial.println("AT+HTTPINIT");
  delay(1000);
  ShowSerialData();

  Serial.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
  ShowSerialData();

  Serial.println("AT+HTTPPARA=\"URL\",\"http://webhook.site/c98b5f5e-2765-470c-a788-f095697c1070\"");
  delay(1000);
  ShowSerialData();

  Serial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(1000);
  ShowSerialData();

  Serial.println("AT+HTTPDATA=" + String(reading.length()) + ",100000");
  delay(1000);
  ShowSerialData;

  Serial.println(reading);
  delay(1000);
  ShowSerialData;

  Serial.println("AT+HTTPACTION=1");
  delay(10000);
  ShowSerialData();

  Serial.println("AT+HTTPREAD");
  delay(1000);
  ShowSerialData();

  Serial.println("AT+HTTPTERM");
  delay(1000);
  ShowSerialData();
  SerialBT.println("Done Anjing");
}