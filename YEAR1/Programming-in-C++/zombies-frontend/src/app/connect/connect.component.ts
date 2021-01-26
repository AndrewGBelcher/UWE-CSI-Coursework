/**
 * Author: Benedict R. Gaster
 * Date: Nov 2017.
 * Desc: Compoenent to ask user to connect to game server and trys to
 * connect to specified server and if successful, then starts game.
 */
import {
  Component,
  Input,
  ElementRef,
  OnInit } from '@angular/core';

import { ActivatedRoute, Router} from '@angular/router';

import { SocketServiceComponent, Output } from '../socket-service/socket-service.component';

/**
 * [Component description]
 * @param  {'app-connect'}               {selector   [description]
 * @param  {'./connect.component.html'}  templateUrl [description]
 * @param  {['./connect.component.css']} styleUrls   [description]
 * @param  {[type]}                      }           [description]
 * @return {[type]}                                  [description]
 */
@Component({
  selector: 'app-connect',
  templateUrl: './connect.component.html',
  styleUrls: ['./connect.component.css'],
})
export class ConnectComponent implements OnInit {

  private errorMsg : string = "";
  private hostPortDefault : string = "127.0.0.1:8085";

  /**
   * [constructor description]
   * @param  {SocketServiceComponent} privatesocketService [description]
   * @return {[type]}                                      [description]
   */
  constructor(
    private socketService: SocketServiceComponent,
    private route: ActivatedRoute,
    private router: Router) {
  }

  /**
   * called when user enters a server address and port, on page's form.
   * Try's to open websocket to specified address, where game server should be
   * running, if successful, then switches to game route, otherwise displays
   * message and get the user to try again.
   * @param {string} hostport represents the host port string enter in form
   */
  private connect(hostport : string) : void {
      this.socketService.connect(hostport).then(connected => {
        if (connected) {
          // connect to game server and so we can play game
          this.router.navigate(["../game"], { relativeTo: this.route } );
        } else {
          // failed to connect to game server so try again
          this.router.navigate(["../connect"], { relativeTo: this.route } );
          this.errorMsg = "<p><b>Failed to connect to server, check address:port and if it is running, then please try again.</b><p>";
        }
      });
    }

  /**
   * [ngOnInit description]
   * @return {[type]} [description]
   */
  ngOnInit() {
  }
}
