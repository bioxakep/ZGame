# ZGame
Зомби квест в реальности.
В данном квесте используется главный модуль на базе Arduino Mega, модуль-мост на базе Arduino Uno, сервер на Python 
на базе Flask и два Processing скрипта в виде оконных мини-приложений, один из которых предназначен для управления 
игрой и отслеживания времени игры, а другой для отображения времени прохождения командами игр.

Главный модуль называется Мастер. К нему подключены все гаджеты (тумблеры, кнопки, датчики), использующиеся в игре 
для взаимодействия с игроками. Он в цикле отслеживает состояние одних и отправляет сигналы на другие с целью 
активации различных эффектов (например звуковых или цветовых, а также отпирание замков и прочее).
В игре несколько этапов или уровней, каждый содержит ряд заданий, которые игроки должны выполнить. 
Время выполнения каждого задания фиксируется, каждый этап игрок может пройти сам либо его может активировать 
оператор с помощью модуля управления (Processing).

Мост (Arduino Uno) используется для передачи информации между Мастером и Processing-модулем управления.
Мастер и Мост соеденены через модуль RS-485, что позволяет разнести их на большое расстояние.
Мост и модуль управления по USB-Serial. Мост в момент запуска сначала соединяется с мониторов оператора, 
затем с Мастером. Соединение с мастером постоянно отслеживается сигналом синхронизации во избежание ситуаций, 
когда один из модулей дал сбой, а игра продолжается. Соединение мастера и моста двунаправленное и каждый может 
после перезагрузки инициализировать соединение.

Модуль, отображающий время прохождения (Scores) также предназначен для ввода имени команды перед началом игры.
При этом после ввода имя отправляется на сервер, где хранится пока игра не закончится.
По окончанию игры модуль управления передает на сервер все параметры игры и они складываются в SQLite базу данных.
Модуль Scores обновляет экран с учетом поступивших в базу обновлений.
