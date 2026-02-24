#import "conf.typ" : conf
#import "@preview/lilaq:0.5.0" as lq
#show: conf.with(
  title: [Лабораторная работа №2],
  type: "referat",
  info: (
      author: (
        name: [Железко Александра Дмитриевича],
        faculty: [КНиИТ],
        group: "251",
        sex: "male"
      ),
      inspector: (
        degree: "Cтарший преподаватель",
        name: "М. И. Сафрончик"
      )
  ),
  settings: (
    title_page: (
      enabled: true
    ),
    contents_page: (
      enabled: false
    )
  )
)

#show: lq.set-diagram(width: 14cm, height: 10cm)
#show lq.selector(lq.diagram): set text(0.8em)
#let xs = (0, 50, 100, 500, 1000)
#align(center, lq.diagram(
  title: [Время выполнения сортировки],
  xlabel: "Количество элементов массива данных, тыс.", 
  ylabel: "Время работы, мс",
  xlim: (0, 1000),
  legend: (position: left, dy: -40%),

  lq.plot(xs, (0, 33, 58, 292, 591), label: [mergeSort]),
  lq.plot(xs, (0, 21, 29, 142, 290), label: [heapSort]),
  lq.plot( xs, (0, 18, 30, 141, 288), label: [quickSort])
)
)