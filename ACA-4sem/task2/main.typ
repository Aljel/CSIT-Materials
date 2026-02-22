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


#let xs = (0, 1, 2, 3, 4)

#lq.diagram(
  title: [Время выполнения сортировки],
  xlabel: "Размер массива данных", 
  ylabel: "Время работы, мс",

  lq.plot(xs, (18, ), mark: "s", label: [mergeSort]),
  lq.plot(
    xs, x => 2*calc.cos(x) + 3, 
    mark: "o", label: [heapSort]
  ),
  lq.plot( 
    xs, (1),
    label: [quickSort]
  )
)