/*
Buvo su go kanalais arba mpi nuo 0-1000 masyvą paskirstyti i 10 gijų po 0-99 100-199 jų vidurkius rast.
B dalis padaryti papildoma metoda kuris skaičiuotu sumų sumas ir issiust i maina
*/

package main

func average(numbers []int, mainChan chan<- float32) {
	sum := 0
	for _, number := range numbers {
		sum += number
	}
	avg := float32(sum) / float32(len(numbers))
	mainChan <- avg
	return
}

func main() {
	const arraySize = 1000
	threadNum := 10

	numbers := [arraySize]int{}
	mainChan := make(chan float32)
	stride := arraySize / threadNum

	for i := 0; i < arraySize; i++ {
		numbers[i] = i + 1
	}
	for i := 0; i < arraySize; i += stride {
		end := i + stride
		if end > arraySize {
			end = arraySize
		}
		numRange := numbers[i:end]
		//fmt.Println(numRange)
		go average(numRange, mainChan)
	}

	totalAvg := float32(0)
	for i := 0; i < threadNum; i++ {
		avg := <-mainChan
		println("Thread avg: ", avg)
		totalAvg += avg
	}
	close(mainChan)
	println("Total average: ", totalAvg)
}
