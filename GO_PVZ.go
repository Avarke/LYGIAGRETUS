/*

Programavimo su go arba mpi a dalis)  5 procesai siuncia skaicius imtinai nuo 0 iki 5*i,
cia i proceso indeksas numeruoti nuo 1, pagrindiniam procesui, kuris sumuoja gautas reiksmes.
Kai gija baigia darba siuncia -1. Programa baigia darba kai visos gijos baige darba

*/

package main

import (
	"fmt"
)

func sendNumbers(k int, writeChan chan<- int) {
	for i := 0; i < 5*k; i++ {
		writeChan <- i
	}
	writeChan <- -1
	return
}

func recieveNumbers(recieveChan <-chan int, doneChan chan<- int, threadNum int) {
	finishedThreads := 0
	sum := 0
	for {
		number := <-recieveChan
		if number >= 0 {
			sum += number
		} else {
			finishedThreads++
		}

		if finishedThreads == threadNum {
			break
		}
	}
	doneChan <- sum
	close(doneChan)
	return
}

func main() {
	threadChan := make(chan int)
	mainChan := make(chan int)
	threadNum := 1
	go recieveNumbers(threadChan, mainChan, threadNum)
	for i := 0; i < threadNum; i++ {
		go sendNumbers(i+1, threadChan)
	}

	sum := <-mainChan
	fmt.Println("Sum: ", sum)

	close(threadChan)
}
