## **Мы встретились вновь, дорогой пир!**

Мы встретились в новом проекте по CI/CD! Желаю тебе погрузиться немножечко в DevOps и насладиться отчетом по данному проекту!

![basic ci/cd image](screenshots/basic_ci_cd.JPG)

## **По-базе, начнем с базы.**

Увы... Если что-то постоянно делается «вручную» – это либо будет работать плохо, либо вовсе не будет.
CI/CD — Это набор принципов и практик, которые позволяют чаще и надежнее развертывать изменения программного обеспечения.

Причины применения CI/CD:

- Командная разработка;
- Длинный жизненный цикл ПО;
- Сокращение релизного цикла;
- Сложность развертывания и тестирования крупных систем;
- Человеческий фактор.

**CI/CD pipeline** - Это последовательность действий (скриптов) для определенной версии кода в репозитории, которая запускается автоматически при совершении изменений.

## **Основы CI**

**CI (Continuous Integration)** — в дословном переводе «непрерывная интеграция».
Имеется в виду интеграция отдельных кусочков кода приложения между собой.
CI обычно выполняет две задачи, описанные далее.

BUILD

- Проверяем, собирается ли вообще код;
- Готовим артефакты для следующих стадий;

TEST

- Тесты кодстайла;
- Модульные тесты;
- Интеграционные тесты;
- Прочие тесты, которые у тебя есть;
- Отчеты о тестах.




## **Основы CD**

**CD (Continuous Delivery)** — это расширение непрерывной интеграции, поскольку оно автоматически развертывает все изменения кода в тестовой и/или производственной среде после этапа сборки.
CD может выполнять задачи, описанные далее.

PUBLISH (В случае применения докера для развёртывания):

- Собираем образы контейнеров;
- Пушим образы туда, откуда их будем потом брать для развертывания;


UPDATE CONFIGS:

- Обновляем конфигурацию на машинах;


DEPLOY STAGING:

- Развертывание тестовой среды для ручных тестов, QA, и прочих не автоматизируемых проверок;
- Может запускаться как вручную, так и автоматически при успешном прохождении стадий CI;


DEPLOY PRODUCTION:

- Разворачиваем новую версию системы на «продакшн»;
- Этот этап желательно запускать вручную, а не автоматически;
- По желанию можно настроить только для определенной ветки репозитория (master, release и т.п.).


## **Part 1. Настройка gitlab-runner**

== Задание ==

**Подними виртуальную машину Ubuntu Server 22.04 LTS.**

![part 1](screenshots/part%201%20ubuntu.png)

[Опционально] Также настроим Port Forwarding для более удобного терминала.

![part 1 port forwarding](screenshots/part%201%20port%20forwarding.png)

![part 1 port frwrd term](screenshots/part%201%20port%20frwrd%20terminal.png)

**Скачай и установи на виртуальную машину gitlab-runner.**

Делаем 
```
sudo apt update
```
Далее, так как GitLab не размещается в стандартных репозиториях Ubuntu, то запускаем скрипт установки, упрощающий процесс

```
curl -L "https://packages.gitlab.com/install/repositories/runner/gitlab-runner/script.deb.sh" | sudo bash
```

После того, когда система знает, откуда "черпать" GitLab Runner, используем команду для установки:

```
sudo apt install gitlab-runner
```

**Запусти gitlab-runner и зарегистрируй его для использования в текущем проекте (DO6_CICD).**

- Для регистрации понадобятся URL и токен, которые можно получить на страничке задания на платформе.

Запускаем команды 
```
sudo gitlab-runner start
sudo gitlab-runner register
```
И указываем в полях ссылку и токен, который у нас указан на странице проекта:

![link and token school 21](screenshots/part%201%20gitlab%20runner%20school%20info.png)

На следующем скрине выделены области, которые спрашивает GitLab-runner: 

- `https://repos.21-school.ru` - адрес сервера GitLab.
- `GR1348941ix...` - токен для регистрации GitLab-runner'a.
- `CI/CD runner` - описание для нашего раннера.
- `codestyle, build, test, deploy` - теги. Рекомендуется максимально точно описывать раннер тегами. С их помощью мы указываем на каких раннерах должны выполняться определенные задачи.
- `shell` - выполняем исполнителя из предложенных вариантов. В нашем случае это просто командный интерпретатор

![registry-with-URL-and-token](screenshots/part%201%20gitlab-runner%20registry.png)

## **Part 2. Сборка**

**== Прелюдия ==**

По-дефолту, в GitLab определено 5 этапов:
- `.pre` — выполняется до всех этапов,
- `.post` — выполняется после всех этапов,
- `build` — первый после `.pre` этап,
- `test` — второй этап,
- `deploy` — третий этап.  

❗️**Порядок, в котором указаны этапы, влияет на порядок, в котором они выполняются.**

> В случае отладки можно получить информацию об окружении, в котором исполняются задачи. `before_script` - список команд, которые будут выполнены до основного списка команд (script). `after_script` - список команд, которые будут выполнены после выполн

Рубрика э-э-э-э-эксперименты!

Попробуем на практике задачу. Нужно сделать так, чтобы при отправке коммитов pipeline запустился. Делается вот так:

```yml
dummy job:
  script:
    - echo ok
```

Запушив все .yml файл, мы получаем job в статусе peding:
![yml file ci/cd pipeline](screenshots/part%202%20cicd%20pipeline.png)

Однако, мы видим ошибку:

![job with error](screenshots/part%202%20job%20screenshot.png)

Для этого мы добавляем тег для нашего GitLab Runner:

```yml
dummy job:
  script:
    - echo ok
  tags:
    - main
```

❗️P.s. [на последнем скрине из Part 1](screenshots/part%201%20gitlab-runner%20registry.png) в разделе тегов у меня не был указан тег `main`, вместе него в .yml я указывал `test`

```yml
dummy job:
  script:
    - echo ok
  tags:
    - test
```

Закоммитив и запушив получаем успешную job:

![job in gitlab](screenshots/part%202%20successed%20job.png)

**== Задание ==**

**Напиши этап для CI по сборке приложений из проекта C2_SimpleBashUtils.**

Начнем с базы: <br>
> *Артефакт* - это общее название любого файла, который был получен в результате сборки. Это может быть результат сборки (jar или exe), отчет о тестах или сгенерированные данные. Другими словами, артефакт - это что-то такое материальное, что не исчезает после окончания pipeline (в отличие от временных файлов, они удаляются по окончании процесса).

`artifacts` - определяет список артефактов сборки; <br>
`artifacts:expire_in` - используется для удаления загруженных артефактов по истечению определенного промежутка времени

Почитать перед сном документацию можно [тут](https://docs.gitlab.com/ee/ci/yaml/index.html#artifacts).

Добавляем проект *C2_SimpleBashUtils*, далее 

В файле gitlab-ci.yml добавь этап запуска сборки через мейк файл из проекта C2.

```yml
script:
    - cd src/cat
    - make s21_cat
    - cd ../grep
    - make s21_grep
```

Файлы, полученные после сборки (артефакты), сохрани в произвольную директорию со сроком хранения 30 дней.

```yml
expire_in: 30 days
```

Сам yml выглядит так:

```yml
stages:
  - build

build-job:
  stage: build
  tags:
    - build
  script:
    - cd src/cat
    - if make s21_cat; then
        echo "✅ s21_cat built successfully.";
      else
        echo "❌ s21_cat built failed.";
        exit 1;
      fi
    - cd ../grep
    - if make s21_grep; then
        echo "✅ s21_grep built successfully.";
      else
        echo "❌ s21_grep built failed.";
        exit 1;
      fi
  when: on_success
  artifacts:
    paths:
      - src/cat/s21_cat
      - src/grep/s21_grep
    expire_in: 30 days
```

Удостоверяемся, что job запускается:

![part 2 job is passed](screenshots/part%202%20build%20job%20is%20edited.png)

Иииииии удостоверяемся, что появились артефакты (Спойлер: они появились)

![aretfacts are added](screenshots/part%202%20artefacts.png)

❗️P.s., не забудь установить все нужные утилиты (gcc, make) на Виртуальную Машину, в противном случае job будет зафейлена:

![disclaimer to install utill p1](screenshots/part%202%20disclaimer%20to%20install%20utill%20part%202.png)
![disclaimer to install utill p2](screenshots/part%202%20disclaimer%20to%20install%20utils%201.png)

❗️P.p.s., по-хорошему еще нужно сообщение о том, что если билд запустился/зафейлился, то нужно отобразить соответствующее собщение через if/else.

## **Part 3. Тест кодстайла**

**== Задание ==**

#### Напиши этап для **CI**, который запускает скрипт кодстайла (*clang-format*).

Добавляем в наш замечательный yml стадию style. Возможно, будет логичнее, чтобы тест на стиль шел в первую очередь. 

❗️ Не забываем установить clang-format, чтобы не было ошибок в job.

```yml
stages:
  - style
  - build

style-test-job:
  stage: style
  tags:
    - codestyle
  script:
    - cd src
    - find . -type f | grep "\.(c|h)$" -E | xargs clang-format -n -verbose --Werror --style=Google

build-job:
  stage: build
  tags:
    - build
  script:
    - cd src/cat
    - if make s21_cat; then
        echo "✅ s21_cat built successfully.";
      else
        echo "❌ s21_cat built failed.";
        exit 1;
      fi
    - cd ../grep
    - if make s21_grep; then
        echo "✅ s21_grep built successfully.";
      else
        echo "❌ s21_grep built failed.";
        exit 1;
      fi
  when: on_success
  artifacts:
    paths:
      - src/cat/s21_cat
      - src/grep/s21_grep
    expire_in: 30 days
```

Помимо этого, еще есть вот такие полезности:

`when` - используется для реализации заданий, которые запускаются в случае сбоя или несмотря на
сбой.
* `on_success` - выполнять задание только в том случае, если все задания предыдущих этапов завершились успешно (или считаются успешными, поскольку они помечены как `allow_failure`). Это значение по умолчанию.
* `on_failure` - выполнять задание только в случае сбоя хотя бы одного задания на предыдущих этапах.
* `always` - выполнять задание независимо от статуса заданий предыдущих этапов.
* `manual` - выполнить задание вручную (добавлено в GitLab 8.10). О ручных действиях читайте ниже.
* `delayed` - выполнить задание через определенный период (добавлено в GitLab 11.14). Подробнее об отложенных действиях читайте ниже.
* `never` - не выполнять задание независимо от статуса заданий на более ранних этапах. Может использоваться только в `rules` разделе или` workflow: rules`.
* `needs` - позволяет выполнять задания вне очереди, что позволяет реализовать направленный ациклический граф в вашем .gitlab-ci.yml.

Почитать можно [тут](https://docs.gitlab.com/ee/ci/yaml/#when) и [тут](https://docs.gitlab.com/ee/ci/yaml/#needs).

#### == *Если кодстайл не прошел, то «зафейли» пайплайн.* ==

Наставили кучу пробелов в s21_grep.h и получили "зайфейленный" pipeline.

![job lists](screenshots/part%203%20job%20lists.png)

![failed pipeline](screenshots/part%203%20failed%20codestyle%20test.png)

#### == *В пайплайне отобрази вывод утилиты *clang-format*.* ==

Меняем стиль, коммитим, пушим и получаем успешный pipeline!

![job lists succeded](screenshots/part%203%20job%20lists%20succeded.png)

![job passed](screenshots/part%203%20output%20of%20the%20pipeline.png)

## **Part 4. Интеграционные тесты**

*== Задание ==*

*== Напиши этап для CI, который запускает твои интеграционные тесты из того же проекта. ==*

Возвращаемся к нашему yml-конфигу и добавляем stage и job:

```yml
stages:
  - style
  - build
  - test

style-test-job:
  stage: style
  tags:
    - codestyle
  script:
    - cd src
    - find . -type f | grep "\.(c|h)$" -E | xargs clang-format -n -verbose --Werror --style=Google
  when: on_success

build-job:
  stage: build
  tags:
    - build
  script:
    - cd src/cat
    - if make s21_cat; then
        echo "✅ s21_cat built successfully.";
      else
        echo "❌ s21_cat built failed.";
        exit 1;
      fi
    - cd ../grep
    - if make s21_grep; then
        echo "✅ s21_grep built successfully.";
      else
        echo "❌ s21_grep built failed.";
        exit 1;
      fi
  when: on_success
  artifacts:
    paths:
      - src/cat/s21_cat
      - src/grep/s21_grep
    expire_in: 30 days

func-test-job:
  stage: test
  tags:
    - test
  script:
    - cd src/cat
    - if bash tests.sh; then
        echo "✅ Tests for s21_cat built successfully!";
      else 
        echo "❌ Ooops! Tests for s21_cat failed!";
        exit 1;
      fi
    - if bash mytests.sh; then
        echo "✅ The 2nd part of the tests for s21_cat built successfully!";
      else 
        echo "❌ Ooops! The 2nd part of the tests for s21_cat failed!";
        exit 1;
      fi
    - cd ../grep
    - if bash tests.sh; then
        echo "✅ Tests for s21_grep built successfully!";
      else 
        echo "❌ Ooops! Tests for s21_grep failed!";
        exit 1;
      fi
    - if bash diffgrep.sh; then
        echo "✅ The 2nd part of the tests for s21_grep built successfully!";
      else 
        echo "❌ Ooops! The 2nd part of the tests for s21_cat failed!";
        exit 1;
      fi
  allow_failure: false
```

*== Запусти этот этап автоматически только при условии, если сборка и тест кодстайла прошли успешно. ==*

Для этого добавляем строчки `when: on_success` (как и указано выше)

*== Если тесты не прошли, то «зафейли» пайплайн. ==*

Добавляем строчки 

```yml
  if bash diffgrep.sh; then
    echo "✅ The 2nd part of the tests for s21_grep built successfully!";
  else 
    echo "❌ Ooops! The 2nd part of the tests for s21_cat failed!";
    exit 1;
  fi
```

Строчка с `exit 1` мы как раз и фейлим pipeline.

*== В пайплайне отобрази вывод, что интеграционные тесты успешно прошли / провалились. ==*

Добавили специальную ошибку в скрипте и получили зафейленный pipeline с соответствующей ошибкой:

![failed pipeline style test](<screenshots/part 4 failed tests.png>)

Исправили ошибку и прогнали еще раз. Все выдает без ошибок: (Happy-Happy-Happy)!

![succeded pipeline style test](<screenshots/part 4 succededd tests.png>)

## **Part 5. Этап деплоя**

== Задание ==

== *Подними вторую виртуальную машину Ubuntu Server 22.04 LTS.* ==

Поднимаем нашу вторую виртуалку:

![part 5 2nd vm](<screenshots/part 5 2nd vm.png>)

== *Напиши этап для CD, который «разворачивает» проект на другой виртуальной машине.* ==

Предварительно залогинившись в терминале Мака под двумя ВМ вспоминаем наши страдания из второго Линукса в виде netplan'a:

![Linux network netplan](<screenshots/part 5 netplan.png>)

Проверяем, что все пингуется:
![netpllan ping](<screenshots/part 5 netplan ping.png>)

Далее с первой машины нужно авторизоваться по ssh на второй. Для этого заходим под пользователем ```sudo su gitlab-runner``` на первую ВМ.

Генерируем ssh-ключ через команду ```ssh-keygen```, читаем директорию ```~/.ssh/id_rsa.pub``` на первой машине и вставляем на вторую в директорию ```~/.ssh/authorized_keys```. <br>
**❗️Обращаю внимание, что ключ надо вставить на вторую ВМ, не заходя в пользователя gitab-runner** <br>
**❗️Надо смотреть, чтобы права у ```~./ssh/id_rsa.pub``` & ```~/.ssh/authorized_keys``` были на чтение. Выдать можно командой ```chmod 644 [название файла]```**

![ssh keygen from 1st to 2nd](<screenshots/part 5 ssh key.png>)

Теперь авторизуемся на первой ВМ через ssh без ввода пароля. 

```ssh den@192.168.100.11```

Тадаааа <br>
![ssh key logged in](<screenshots/part 5 1st vm logged.png>)


== *Напиши bash-скрипт, который при помощи ssh и scp копирует файлы, полученные после сборки (артефакты), в директорию /usr/local/bin второй виртуальной машины. Тут тебе могут помочь знания, полученные в проекте DO2_LinuxNetwork.* ==

Для этого на второй ВМ создаем скрипт ```build.sh```:

![part 5 build script](<screenshots/part 5 build script.png>)

Выдаем права на выполнение:
```sudo chmod +x build.sh```

План заключался в том, чтобы поставить владельцем root и установить suid (систему разрешения, позволяющую запускать исполняемый файл с правами владельца этого файла), чтобы разрешить выполнение от имени root. Однако, suid не работает в shell-скриптах.

```
sudo chown root:root build.sh
sudo chmod 4755 build.sh
```

Так как suid не работает, тогда в `/etc/sudoers` добавляем строчку

```
den ALL = (root) NOPASSWD: /home/den/build.sh
```

![part 5 adding to sudoers](<screenshots/part 5 sudoers.png>)

Теперь пользователь `den` может запускать скрипт без ввода пароля.

== *Будь готов объяснить по скрипту, как происходит перенос.* ==

Всегда готов 😎

== *Запусти этот этап вручную при условии, что все предыдущие этапы прошли успешно.* ==

В наш замечательный yml конфиг добавелям стадию deploy

```yml
stages:
  - style
  - build
  - test
  - deploy

style-test-job:
  stage: style
  tags:
    - codestyle
  script:
    - cd src
    - find . -type f | grep "\.(c|h)$" -E | xargs clang-format -n -verbose --Werror --style=Google
  when: on_success

build-job:
  stage: build
  tags:
    - build
  script:
    - cd src/cat
    - if make s21_cat; then
        echo "✅ s21_cat built successfully.";
      else
        echo "❌ s21_cat built failed.";
        exit 1;
      fi
    - cd ../grep
    - if make s21_grep; then
        echo "✅ s21_grep built successfully.";
      else
        echo "❌ s21_grep built failed.";
        exit 1;
      fi
  when: on_success
  artifacts:
    paths:
      - src/cat/s21_cat
      - src/grep/s21_grep
    expire_in: 30 days

func-test-job:
  stage: test
  tags:
    - test
  script:
    - cd src/cat
    - if bash tests.sh; then
        echo "✅ Tests for s21_cat built successfully!";
      else 
        echo "❌ Ooops! Tests for s21_cat failed!";
        exit 1;
      fi
    - if bash mytests.sh; then
        echo "✅ The 2nd part of the tests for s21_cat built successfully!";
      else 
        echo "❌ Ooops! The 2nd part of the tests for s21_cat failed!";
        exit 1;
      fi
    - cd ../grep
    - if bash tests.sh; then
        echo "✅ Tests for s21_grep built successfully!";
      else 
        echo "❌ Ooops! Tests for s21_grep failed!";
        exit 1;
      fi
    - if bash diffgrep.sh; then
        echo "✅ The 2nd part of the tests for s21_grep built successfully!";
      else 
        echo "❌ Ooops! The 2nd part of the tests for s21_cat failed!";
        exit 1;
      fi
  allow_failure: false

deploy-job:
  stage: deploy
  tags:
    - deploy
  when: manual
```

== *В файле gitlab-ci.yml добавь этап запуска написанного скрипта.* ==

Добавляем еще строчки:

```yml
  script:
    - scp src/cat/s21_cat src/grep/s21_grep den@192.168.100.11:~/builds/
    - ssh den@192.168.100.11 'sudo ~/build.sh'
```

Пушим и убеждаемся, что deploy job запускается вручную:

![blocked deploy job](<screenshots/part 5 blocked pipeline.png>)

![deploy job](<screenshots/part 5 deploy job.png>)

== *В случае ошибки «зафейли» пайплайн. В результате ты должен получить готовые к работе приложения из проекта C2_SimpleBashUtils (s21_cat и s21_grep) на второй виртуальной машине.* ==

Делается это дело несложно. Добавляем в наш конфиг строчки с if-else:

```yml
script:
    - if scp src/cat/s21_cat src/grep/s21_grep den@192.168.100.11:~/builds; then
        echo "✅ Files successfully copied to remote server.";
      else
        echo "❌ Failed to copy files to remote server.";
        exit 1;
      fi
    - if ssh den@192.168.100.11 'sudo ~/build.sh'; then
        echo "✅ Remote build script executed successfully.";
      else
        echo "❌ Failed to execute remote build script.";
        exit 1;
      fi
```
Проверяем, что сообщение показывается в случае успеха:

![script output success](<screenshots/part 5 script output.png>)

И в случае провала:

![part 5 exeecuted script error](<screenshots/part 5 executed script error.png>)

Удостоверяемся, что бинарные файлы появились в ```/usr/local/bin```:

![artifacts in usr local bin](<screenshots/part 5 checking if there are binaries in directory.png>)

== *Сохрани дампы образов виртуальных машин. P.S. Ни в коем случае не сохраняй дампы в гит!* ==

![dumps](<screenshots/part 5 dumps.png>)

== *Не забудь запустить пайплайн с последним коммитом в репозитории.* ==

Всё есть!

![proof that is the latest commit](<screenshots/part 5 proof that it is the latest repo.png>)

❗️P.s., если вдруг вы столкнетесь с проблемой:

![fatal error](<screenshots/part 5 fatal error.jpg>)

Если тебе job выплевывает ошибку на стадии тестов (однако проблем ни с yml конфигом, ни с чем другим не наблюдалось) - то возвращайся к [пункту 1](#part-1-настройка-gitlab-runner), отмени регистрацию при помощи ```gitlab-runner unregister --url [ТВОЯ_ССЫЛКА_ПРИ_РЕГИСТРАЦИИ] --token [ТОКЕН]``` и заново сделай регистрацию.

❗️ПРИМЕЧАНИЕ: токен ты можешь найти в директории при помощи: 
```
sudo find / -name config.toml
```
Обычно он лежит в директории ```/etc/gitlab-runner/```

## **Part 6. Дополнительно. Уведомления**

== Задание ==

== *Настрой уведомления о успешном/неуспешном выполнении пайплайна через бота с именем «[твой nickname] DO6 CI/CD» в Telegram.* ==

== *Текст уведомления должен содержать информацию об успешности прохождения как этапа CI, так и этапа CD. В остальном текст уведомления может быть произвольным.* ==

Let's get down to the business then!

Для этого в Telegram находим **BotFather** и нажимаем на опцию ```/newbot```

![comand to create a bot](<screenshots/part 6 newbot.png>)

Следуя инструкциям получаем от него **токен**, который понадобится для нашего bash-скрипта.

Также для скрипта понадобится наш ID (нет, не тот, который через @). Два варианта развития: 
1. Узнаем свой ID открыв в браузере телегу к себе в личные сообщения (saved messages), находим в поисковой строке девятизначные цифры - это и есть наш ID
2. Находим в Телеграмме бота @userinfobot и узнаем через него свой ID.

Далее пишем bash-скрипт и добавляем в наш yml-конфиг строчки after_script:

```yml
after_script:
      - bash src/bot.sh
```

Все пушим и проверям:

![passed pipeline](<screenshots/part 6 successfull pipeine.png>)

![tg pipeline](<screenshots/part 6 tg successful pipeline.png>)

Специально "фейлим" пайплайн

![failed pipeline](<screenshots/part 6 failed pipeline.png>)

![telegram notofication failed pipeline](<screenshots/part 6 tg failed pipeline.png>)

Вот и весь проект! Всё!