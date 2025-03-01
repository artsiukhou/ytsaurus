# Квоты

В данном разделе приведено описание квот на ресурсы в системе {{product-name}}.

## Типы ресурсов { #resources }

Большинство кластеров {{product-name}} являются кластерами общего пользования. Ресурсы кластеров являются разделяемыми для всех проектов, которым требуется хранение и обработка данных. Данные квоты отображаются, но не включены.
Можно выделить следующие типы квот на ресурсы:

* Хранение:
  * дисковая квота на HDD (SATA) без учёта сжатия и репликации;
  * дисковая квота на SATA SSD/NVMe SSD без учёта сжатия и репликации;
  * квота на количество узлов (nodes) в [дереве метаинформации](../../../user-guide/storage/cypress.md) (таблиц, файлов, блокировок), при годовом планировании отдельно не заказывается, выдаётся по договорённости. Имеется ограничение на число узлов дерева на весь кластер;
  * квота на количество чанков (chunks) на которые делятся все хранимые данные. Выдаётся пропорционально дисковым квотам исходя из размера 512Мб на чанк, плюс запас в 200 000 чанков или по договоренности с проектом;
  * квота на объем оперативной памяти мастер-серверов, потребляемый узлами (nodes) в дереве метаинформации;  отдельно не заказывается, выдаётся по договорённости. Учитываются метаданные чанков и узлов, атрибуты узлов, pivot-ключи [динамических таблиц](../../../user-guide/dynamic-tables/overview.md), а также некоторые другие данные. Эмпирическая оценка: 100-200 байт на узел и столько же на чанк. Указанные величины могут существенно варьироваться в зависимости от длины атрибутов и имён узлов, длины значений в ключевых колонках чанков и т.д.;
  * квота на количество [таблетов (шардов)](../../../user-guide/dynamic-tables/overview.md#tablets) динамических таблиц. Отдельно не заказывается, выдаётся по запросу. Квота выделяется на объект [таблет-селл бандл (tablet_cell_bundle)](../../../user-guide/dynamic-tables/concepts.md#tablet_cell_bundles)
* Обработка:
  * гарантированное число HyperThreading ядер для MapReduce операций;
  * гарантированный объём оперативной памяти для MapReduce операций;
  * ограничение на количество выполняющихся MapReduce операций и общее количество операций, включая операции, стоящие в очереди на запуск;
  * выделенные вычислительные мощности для выполнения запросов к [динамическим таблицам](../../../user-guide/dynamic-tables/overview.md) (write/select/lookup) — CPU, RAM. Сюда входит квота на объём данных в динамических таблицах, поднятых в память (tablet_static_memory). Квоты выделяются на объект [таблет-селл бандл (tablet_cell_bundle)](../../../user-guide/dynamic-tables/concepts.md#tablet_cell_bundles).

## Хранение данных { #account_quotas }

Большинство квот для хранения данных привязаны к **аккаунту** проекта. Аккаунт — это служебный объект в Кипарисе. Каждый узел Кипариса имеет атрибут `account`, доступный на чтение и запись, в котором хранится имя аккаунта, к которому привязан данный узел. В случае файлов и таблиц назначение аккаунта влечет за собой расход его дисковой квоты. При этом учитывается пространство, физически выделенное на узлах кластера для хранения данных, поэтому оно зависит от степени репликации (атрибут `replication_factor`), способа erasure-кодирования (атрибут `erasure_codec`) и используемой компрессии (атрибут `compression_codec`).

В случае превышения пользователем квоты аккаунта по отдельным видам ресурсов система начинает отказывать в дальнейшем использовании аккаунта по превышенному виду ресурса. Это не означает, что использование аккаунта никогда не превышает лимитов. Например, лимиты могли быть принудительно выставлены ниже использования. Кроме того, отслеживание потребления дисковой и чанковой квот работает асинхронно, что также может выливаться в превышение отведенных лимитов.

Подробнее можно прочитать в разделе [Аккаунты](../../../user-guide/storage/accounts.md).

### Медиумы { #medium }

Различные типы носителей (HDD, SSD, RAM) логически объединены в специальные сущности, называемые [медиумами](../../../user-guide/storage/media.md) (medium). Типы носителей отличаются по объёму доступного пространства и скорости чтения/записи. Чем выше скорость носителя, тем меньше его объём. Медиумы разделяют всё множество чанковых локаций на непересекающиеся подмножества. При этом один узел кластера может иметь локации, относящиеся к разным медиумам. В пределах каждого такого подмножества осуществляется независимая балансировка чанков и независимое квотирование. При выдаче дисковой квоты аккаунту, квота выдаётся в конкретном медиуме. Пользователь может выбирать на каком медиуме хранить те или иные данные. На кластерах {{product-name}} распространены следующие виды медиумов:

- default — объединяет множество HDD дисков в кластере;
- ssd_blobs — множество SSD дисков в кластере;
- ssd_journals — множество SSD дисков для хранения журналов динамических таблиц;
- in_memory — выделенное пространство в оперативной памяти узлов кластера.

Для просмотра квот аккаунта существует специальная страница в веб-интерфейсе {{product-name}}.

Подробнее о медиумах можно прочитать в разделе [Медиумы](../../../user-guide/storage/media.md).

### Ресурсы динамических таблиц { #dyntable_resources }

Квоты на количество таблетов и tablet static memory привязаны к таблет-селл бандлам (tablet cell bundle). Квоты бандла отображаются в отдельном разделе веб-интерфейса. Настройка мониторинга и просмотр истории значений квот аккаунтов и бандлов рассмотрены в разделе Мониторинг квот.

## Обработка данных { #data_processing }

Вычислительные мощности для обработки данных привязаны к [пулам](../../../user-guide/data-processing/scheduler/scheduler-and-pools.md) (кроме tablet_cell_bundle).
Настройки вычислительного пула, включая гарантии и лимиты на количество операций можно посмотреть на странице **Scheduling**, вкладка **Details**, раздел **CPU**, колонка `Strong guar`.

{% if audience == "public" %}

В том случае, когда проект использует динамические таблицы в production, для изоляции от других пользователей необходимо завести отдельный `tablet_cell_bundle`. [Tablet_cell_bundle](../../../user-guide/dynamic-tables/concepts.md#tablet_cell_bundles) представляет собой группу контейнеров (инстансов), которая отвечает за обработку запросов к динамическим таблицам проекта.

{% endif %}

{% note info "Примечание" %}

Для работы динамических таблиц требуется дисковая квота для записи журналов (medium ssd_journals) и снапшотов (medium default), выдаётся автоматически. Требуется квота на объекты файловой системы (node_count), а также квота на количество таблетов (шардов таблицы — tablet_count).

{% endnote %}

{% if audience == "internal" %}

{% note warning "Внимание" %}

Пользователи должны самостоятельно следить за квотами на выделенные ресурсы и своевременно заказывать их увеличение при необходимости.

{% endnote %}

{% endif %}
