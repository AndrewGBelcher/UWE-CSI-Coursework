/**
 *
 */
import { Injectable, Input } from '@angular/core';
import { Observable, Observer, BehaviorSubject } from 'rxjs';

/**
 *
 */
export interface Score {
  score : number;
}

/**
 *
 */
export interface Output {
  output : string;
}

/**
 *
 */
export interface Timer {
  value : number;
}

/**
 *
 */
interface Message {
  type: string;
  value?: number; // if a score msg
  text?: string; // if a output msg
}

/**
 * [Injectable description]
 * @return {[type]} [description]
 */
@Injectable()
export class SocketServiceComponent {

  private scores : BehaviorSubject<Score> =
    new BehaviorSubject({ "score" : 0 });

  private outputs : BehaviorSubject<Output> =
    new BehaviorSubject({ "output" : ""});

  private timers : BehaviorSubject<Timer> =
    new BehaviorSubject({ "value" : 0 });

  private url : string = "127.0.0.1"; //localhost

  private onConnectMsg : string = "begin";

  private ws : WebSocket;

  /**
   * [constructor description]
   * @return {[type]} [description]
   */
  constructor() {
  }

  public connect(hostPort : string) : Promise<boolean>{

    this.ws = new WebSocket("ws://" + hostPort);

    this.ws.onmessage = (event) => {
        console.log(event.data);
        var msg : Message = JSON.parse(event.data);
        switch(msg.type) {
          case "score":
            if (msg.value) {
              this.scores.next({ "score" : msg.value });
            }
            break;
          case "output":
            if (msg.text) {
              console.log("output: " + msg.text);
              this.outputs.next({ "output" : msg.text });
            }
            break;
          case "timer":
            this.timers.next({ "value" : msg.value });
            break;
        }
    };

    // return a promise that allows the caller to determine if and when the
    // connection is made
    return new Promise( resolve => {
      this.ws.onerror = function (error) {
        console.log("error");
          resolve(false);
        };

      this.ws.addEventListener("open", event => {
           console.log("open");
           this.ws.send(this.onConnectMsg);
           resolve(true);
      });
    });
  }

  /**
   * [scoreStream description]
   * @return {Observable<Score>} [description]
   */
  public scoreStream() : Observable<Score> {
    return this.scores.asObservable();
  }

  /**
   * [outputStream description]
   * @return {Observable<Output>} [description]
   */
  public outputStream() : Observable<Output> {
    return this.outputs.asObservable();
  }

  /**
   * [timerStream description]
   * @return {Observable<Timer>} [description]
   */
  public timerStream() : Observable<Timer> {
    return this.timers.asObservable();
  }

  /**
   * [send description]
   * @param {string} msg [description]
   */
  public send(msg: string) : void {
    this.ws.send(msg);
  }
}
