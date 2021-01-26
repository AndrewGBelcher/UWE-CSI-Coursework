import { LorawanBristolPage } from './app.po';

describe('lorawan-bristol App', function() {
  let page: LorawanBristolPage;

  beforeEach(() => {
    page = new LorawanBristolPage();
  });

  it('should display message saying app works', () => {
    page.navigateTo();
    expect(page.getParagraphText()).toEqual('app works!');
  });
});
